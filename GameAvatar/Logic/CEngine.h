#ifndef _CENGINE_H
#define _CENGINE_H

//#include "CSocketHolder.h"
#include "CWinThread.h"
#include "CICommand.h"
#include "CResourcesLoader.h"
#include <Windows.h>
#include "GL/glut.h"

class CEngine {
public:
	// main constructor
	CEngine();
	// dtor
	~CEngine();
	void connectionLoop();
	// main method to be called from main.cpp
	void ignition();
	// bridge - so this way we may wrap all opengl code and make it OOP
	static CEngine* m_pInstance;
	// just set m_pInstance to this object
	void instance();
private:
	void execute();
	void reshape(TInt32 w, TInt32 h);
	static void RunWrap();
	static void ReshapeWrap(TInt32 w, TInt32 h);
	static void KeyboardInput(TUByte key, int x, int y);
	static void KeyboardRelease(TUByte key, int x, int y);
	static void MouseInput(int button, int state, int x, int y);
	static void MouseMotion(int x, int y);
	static DWORD WINAPI ConnectionListener(LPVOID lpParameter);
	static DWORD WINAPI BackgroundLoader(LPVOID lpParameter);

	static bool m_bTerminateApplication;

	//CSocketHolder m_socketServer;
	CResourcesLoader m_resourceLoader;

	// mouse members - to be moved somewhere else
	static int s_lastState;
	static int s_lastCursorX;
	static int s_lastCursorY;

};

#endif //_CENGINE_H

