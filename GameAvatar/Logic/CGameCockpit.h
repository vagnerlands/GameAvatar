#ifndef _CGAMECOCKPIT_H_
#define _CGAMECOCKPIT_H_

#include <string>
#include <iostream>
#include <list>
#include <memory>
#include "CCommonTypes.h"
#include "CWinSocket.h"
#include "CPerson.h"
#include "CControllerMenu.h"
#include "IEvent.h"
#include "CViewElementSquare.h"
#include "CViewElementModel.h"
#include "CHumanView.h"
#include "IView.h"
#include "IProcess.h"
#include "IMutex.h"

#include "GL/glut.h"

using namespace std;
using namespace Types;

class CGameCockpit {
public:
	static CGameCockpit* instance();

	bool validateUserLogin(string socketKey, string login, bool* isCharacterCreated);

	void setClass(string socketKey, ECharacterClass charClass);
	// adds a process request in the process queue
	void pushProcess(shared_ptr<IProcess> pProcess);
	// removes a given process from the Process Queue
	void popProcess(shared_ptr<IProcess> pProcess);

	shared_ptr<IProcess> getNextProcess();

	static void ControllerMenuEventHandler(IEvent* ev);

	void run();
	CControllerMenu m_CtrlMenu;
private:
	CGameCockpit();
	~CGameCockpit();
	static CGameCockpit* s_pInstance;

	list<CPerson*> m_userDB;
	ViewList m_views;

	// list of processes for background thread
	list<shared_ptr<IProcess>> m_processes;
	// mutex for m_processes
	IMutex* m_processesMutex;
	
};

#endif //_CGAMELOGIN_H_
