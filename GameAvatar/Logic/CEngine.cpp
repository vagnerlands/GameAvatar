#include "CEngine.h"
#include "CThreadHolder.h"
#include "CEventManager.h"
#include <iostream>

using namespace std;

CEngine* CEngine::m_pInstance = nullptr;

void
CEngine::instance() {
	// just set the instance 
	m_pInstance = this;
}

CEngine::CEngine()
{

}

CEngine::~CEngine() {
	//empty implementation
}
void 
CEngine::connectionLoop()
{
	cout << "Accepting new connection..." << endl;
	m_socketServer.onConnectionEvent();
}


// @
// Execute a given command - output shall be
// @
void
CEngine::execute() 
{
	// read incoming bytes from all sockets
	m_socketServer.readIncomingMsgs();

	// execute commands from Sockets
	m_socketServer.executeCommands();

	// runs periodic functions based on system events
	CGameCockpit::instance()->run();

	// remove all dead sockets
	m_socketServer.removeCondemedSockets();
}

void
CEngine::reshape(TInt32 w, TInt32 h)
{
	glViewport(0, 0, (GLsizei)s_SCREEN_WIDTH, (GLsizei)s_SCREEN_HEIGHT);
}

void 
CEngine::RunWrap() 
{
	// performs main logic features
	m_pInstance->execute();
}

void 
CEngine::ReshapeWrap(TInt32 w, TInt32 h)
{
	m_pInstance->reshape(w, h);
}

DWORD WINAPI
CEngine::ConnectionListener(LPVOID lpParameter)
{
	// remains listening for more connections "forever"
	// or till the system runs out of resources...
	while (true)
	{
		// performs main logic features
		m_pInstance->connectionLoop();
	}
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
	CThreadHolder::instance()->registerThread("thSocketListener", ConnectionListener);

	// declares the drawing function to opengl state machine
	glutDisplayFunc(RunWrap);
	// declares the reshape function to opengl state machine
	glutReshapeFunc(ReshapeWrap);
	// declares the idle function to opengl state machine
	glutIdleFunc(RunWrap);
	
	// initialize the opengl main loop
	// this will handle the whole displaying states of our game...
	glutMainLoop();

	// releases the <listening> thread
	CThreadHolder::instance()->destroyThread("thSocketListener");

	return ;
}
