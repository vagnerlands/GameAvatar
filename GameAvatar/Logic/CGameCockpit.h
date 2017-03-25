#ifndef _CGAMECOCKPIT_H_
#define _CGAMECOCKPIT_H_

#include <string>
#include <iostream>
#include <list>
#include "CCommonTypes.h"
#include "CWinSocket.h"
#include "CPerson.h"
#include "CControllerMenu.h"
#include "IEvent.h"
#include "CViewElementSquare.h"
#include "CHumanView.h"
#include "IView.h"

#include "GL/glut.h"

using namespace std;
using namespace Types;

class CGameCockpit {
public:
	static CGameCockpit* instance();

	bool validateUserLogin(string socketKey, string login, bool* isCharacterCreated);

	void setClass(string socketKey, ECharacterClass charClass);

	static void ControllerMenuEventHandler(IEvent* ev);

	void run();
	CControllerMenu m_CtrlMenu;
private:
	CGameCockpit();
	~CGameCockpit();
	static CGameCockpit* s_pInstance;

	list<CPerson*> m_userDB;
	ViewList m_views;
	
};

#endif //_CGAMELOGIN_H_
