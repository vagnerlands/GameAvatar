#include "CEngine.h"
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
	// read incoming bytes from all sockets
	//m_socketServer.readIncomingMsgs();

	// execute commands from Sockets
	//m_socketServer.executeCommands();

	// prepares ORTHO PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //Reset the drawing perspective
	//glOrtho(-(s_SCREEN_WIDTH / 2.0), (s_SCREEN_WIDTH / 2.0), -(s_SCREEN_HEIGHT / 2.0), (s_SCREEN_HEIGHT / 2.0), -600.0, 600.0);
	//glFrustum(-(s_SCREEN_WIDTH / 2.0), (s_SCREEN_WIDTH / 2.0), -(s_SCREEN_HEIGHT / 2.0), (s_SCREEN_HEIGHT / 2.0), 0.1f, 60000.0f);
	static GLfloat frustumParams[] = {-1.f, 1.f, -1.f, 1.f, 5.f, 10000.f};
	glFrustum(frustumParams[0], 
		frustumParams[1], 
		frustumParams[2], 
		frustumParams[3], 
		frustumParams[4], 
		frustumParams[5]);
	gluLookAt(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.f, 0.0f);
	// prepares MODEL VIEW
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// Set background (clear) color to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//GLenum err = glGetError();
	//glMatrixMode(GL_PROJECTION);                // update projection
	//err = glGetError();

	// runs periodic functions based on system events
	CGameCockpit::instance()->run();

	// remove all dead sockets
	//m_socketServer.removeCondemedSockets();

	if (m_bTerminateApplication)
	{
		CGameCockpit::instance()->Close();
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

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

								 //Set the camera perspective
	glLoadIdentity(); //Reset the camera
	//gluPerspective(45.0f,                  //The camera angle
	//	(double)s_SCREEN_WIDTH / (double)s_SCREEN_HEIGHT, //The width-to-height ratio
	//	0.1f,                   //The near z clipping coordinate
	//	60000.0f);                //The far z clipping coordinate
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
	CGameCockpit::instance()->getGameController()->VOnKeyDown(key);
}

// reads the user input key - KEY UP
void CEngine::KeyboardRelease(TUByte key, int x, int y)
{
	// if ESC key - application must be terminated
	if (key == 27) 
		m_bTerminateApplication = true;
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

	glutGameModeString("800x600:32@60");
	
	// initialize the opengl main loop
	// this will handle the whole displaying states of our game...
	glutMainLoop();

	CThreadHolder::instance()->destroyThread("thBackgroundLoader");
	// releases the <listening> thread
	//CThreadHolder::instance()->destroyThread("thSocketListener");

	return ;
}
