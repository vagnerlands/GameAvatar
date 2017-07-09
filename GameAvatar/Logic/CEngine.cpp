#include "CEngine.h"
#include "CShaderManager.h"
#include "CModelManager.h"
#include "CGameCockpit.h"
#include "CThreadHolder.h"
#include "CEventManager.h"
#include <iostream>
//#define _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;

CEngine* CEngine::m_pInstance = nullptr;

bool CEngine::m_bTerminateApplication = false;

int CEngine::s_lastState = GLUT_UP;
int CEngine::s_lastCursorX = -1;
int CEngine::s_lastCursorY = -1;


void
CEngine::instance() {
	// just set the instance 
	m_pInstance = this;
}

CEngine::CEngine()
{

}

CEngine::~CEngine() 
{
	//empty implementation
}
void 
CEngine::connectionLoop()
{
	cout << "Accepting new connection..." << endl;
	//m_socketServer.onConnectionEvent();
}


// @
// Execute a given command - output shall be
// @
void
CEngine::execute() 
{
	// runs periodic functions based on system events
	CGameCockpit::instance()->run();

	// remove all dead sockets
	//m_socketServer.removeCondemedSockets();

	if (m_bTerminateApplication)
	{
		CGameCockpit::instance()->Close();
		delete CModelManager::instance();
		delete CTextManager::instance();
		delete CShaderManager::instance();
		//delete m_pInstance;
	}

	TInt32 err = glGetError();
	while (err != 0)
	{
		printf("glError Unknown error = %d\n", err);
		err = glGetError();
	}

	glutSwapBuffers();
}

void
CEngine::reshape(TInt32 w, TInt32 h)
{
	glViewport(0, 0, (GLsizei)s_SCREEN_WIDTH, (GLsizei)s_SCREEN_HEIGHT);

	// prepare Projection matrix
	glMatrixMode(GL_PROJECTION); 

	// Set identity matrix
	glLoadIdentity(); 

}

void 
CEngine::RunWrap() 
{
	// performs main logic features
	m_pInstance->execute();

	if (m_bTerminateApplication)
	{
		//delete m_pInstance;
		exit(1);
	}
}

void 
CEngine::ReshapeWrap(TInt32 w, TInt32 h)
{
	m_pInstance->reshape(w, h);
}

// reads the user input key - KEY DOWN
void CEngine::KeyboardInput(TUByte key, int x, int y)
{
	if (key == 'c')
	{
		s_lastState = GLUT_DOWN;
		s_lastCursorX = -1;
		s_lastCursorY = -1;
	}
	CGameCockpit::instance()->getGameController()->VOnKeyDown(key);
}

void CEngine::MouseMotion(int x, int y)
{
	float cursorX, cursorY = 0.0f;

	if (s_lastCursorX == -1)
	{
		s_lastCursorX = x;
	}

	if (s_lastCursorY == -1)
	{
		s_lastCursorY = y;
	}

	if (s_lastState == GLUT_DOWN) 
	{
		cursorX = x;
		cursorY = y;

		float deltaX = 0;
		float deltaY = 0;

		deltaX = s_lastCursorX - cursorX;
		deltaY = s_lastCursorY - cursorY;

		float angle = (int)((atan2(deltaY, deltaX) * 180 / 3.14f) + 270) % 360;

		if ((s_lastCursorX != cursorX) || (s_lastCursorY != cursorY))
		{
			printf("angle=%f, deltaX = %f, deltaY = %f\n",
				angle,
				deltaX,
				deltaY);

			s_lastCursorX = cursorX;
			s_lastCursorY = cursorY;

			if (deltaX > 0)
			{
				deltaX *= -1;
			}

			if (deltaY < 0)
			{
				deltaY *= -1;
			}

			CCamera* tmp = &CGameCockpit::instance()->m_camera;

			tmp->RotateX(cos(angle * 3.14 / 180) * (deltaY / 10.f));
			tmp->RotateY(sin(angle * 3.14 / 180) * (deltaX / 10.f));
		}
	}

	printf("Mouse @ %d %d\n", x, y);


}

void CEngine::MouseInput(int button, int state, int x, int y)
{
	if (s_lastCursorX == -1)
	{
		s_lastCursorX = x;
	}

	if (s_lastCursorY == -1)
	{
		s_lastCursorY = y;
	}


	// Wheel reports as button 3(scroll up) and button 4(scroll down)
	if ((button == 3) || (button == 4)) // It's a wheel event
	{
		// Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
		printf("Scroll %s At %d %d\n", (button == 3) ? "Up" : "Down", x, y);
		if (button == 3)
		{
			CGameCockpit::instance()->m_camera.MoveForward(1.0f);
		}
		else
		{
			CGameCockpit::instance()->m_camera.MoveForward(-1.0f);
		}
	}
	else 
	{  // normal button event
		printf("Button %d - %s At %d %d\n", button, (state == GLUT_DOWN) ? "Down" : "Up", x, y);
	}


	

	s_lastCursorX = x;
	s_lastCursorY = y;
	//s_lastState = state;

	//glutPostRedisplay();
}

// reads the user input key - KEY UP
void CEngine::KeyboardRelease(TUByte key, int x, int y)
{
	// if ESC key - application must be terminated
	if (key == 27) 
		m_bTerminateApplication = true;
	if (key == 'c')     s_lastState = GLUT_UP;
	if (key == '9') 	glutEnterGameMode();
	if (key == '0') 	glutLeaveGameMode();
	CGameCockpit::instance()->getGameController()->VOnKeyUp(key);
}

DWORD WINAPI
CEngine::ConnectionListener(LPVOID lpParameter)
{
	// remains listening for more connections "forever"
	// or till the system runs out of resources...
	while (!m_bTerminateApplication)
	{
		// performs main logic features
		m_pInstance->connectionLoop();
	}

	return (DWORD WINAPI)0;
}

DWORD WINAPI
CEngine::BackgroundLoader(LPVOID lpParameter)
{
	// remains listening for more resource load requests
	while (!m_bTerminateApplication)
	{
		// load resources in background thread
		m_pInstance->m_resourceLoader.execute();
	}

	return (DWORD WINAPI)0;
}


void 
CEngine::ignition() 
{
	// first thing ever - set "this" instance to the m_pInstance
	instance();

	// initialize all singletons

	// register event listeners
	CEventManager::instance()->registerListener(EEVENTTYPE_MENU_STATE, &CGameCockpit::ControllerMenuEventHandler);

	// creates thread incomming "TCP/IP connection requests"
	//CThreadHolder::instance()->registerThread("thSocketListener", ConnectionListener);
	// creates a thread for loading resources purpose
	CThreadHolder::instance()->registerThread("thBackgroundLoader", BackgroundLoader);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	// declares the drawing function to opengl state machine
	glutDisplayFunc(RunWrap);
	// declares the reshape function to opengl state machine
	glutReshapeFunc(ReshapeWrap);
	// declares the idle function to opengl state machine
	glutIdleFunc(RunWrap);
	// key press down event
	glutKeyboardFunc(KeyboardInput);
	// key press up event
	glutKeyboardUpFunc(KeyboardRelease);
	// mouse click input
	glutMouseFunc(MouseInput);
	// mouse motion input
	glutPassiveMotionFunc(MouseMotion);

	glutGameModeString("800x600:32@60");

	// initialize shaders
	CShaderManager::instance()->LoadShader("simple");
	CShaderManager::instance()->LoadShader("light");
	CShaderManager::instance()->LoadShader("model");
	CShaderManager::instance()->LoadShader("quasicrystal");
	CShaderManager::instance()->LoadShader("noise");
	
	// initialize the opengl main loop
	// this will handle the whole displaying states of our game...
	glutMainLoop();

	CThreadHolder::instance()->destroyThread("thBackgroundLoader");
	// releases the <listening> thread
	//CThreadHolder::instance()->destroyThread("thSocketListener");

	return ;
}
