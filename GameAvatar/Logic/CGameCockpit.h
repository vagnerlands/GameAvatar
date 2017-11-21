#ifndef _CGAMECOCKPIT_H_
#define _CGAMECOCKPIT_H_

#include <string>
#include <iostream>
#include <list>
#include <memory>
#include "CCommonTypes.h"
#include "CControllerMenu.h"
#include "IEvent.h"
#include "CViewElementSquare.h"
#include "CViewElementModel.h"
#include "CViewElementTerrainMesh.h"
#include "CViewElementHUD.h"
#include "CHumanView.h"
#include "IView.h"
#include "IProcess.h"
#include "IMutex.h"
#include "CGameController.h"
#include "CCamera.h"
#include "Skybox.h"

#include "GL/glut.h"

using namespace std;
using namespace Types;

class CGameCockpit {
public:

	CSkybox* m_objSkyBox;
	static CGameCockpit* instance();

	// for application closure
	void Close();

	//bool validateUserLogin(string socketKey, string login, bool* isCharacterCreated);

	//void setClass(string socketKey, ECharacterClass charClass);
	// adds a process request in the process queue
	void pushProcess(shared_ptr<IProcess> pProcess);
	// removes a given process from the Process Queue
	void popProcess(shared_ptr<IProcess> pProcess);

	// grants access to CGameController instance in CGameCockpit
	// to set which keys the user is pressing
	shared_ptr<CGameController> getGameController();

	shared_ptr<IProcess> getNextProcess();

	static void ControllerMenuEventHandler(IEvent* ev);

	static void UserInputEventHandler(IEvent* ev);

	void run();

	CControllerMenu m_CtrlMenu;

	CCamera m_camera;

private:
	CGameCockpit();
	~CGameCockpit();
	static CGameCockpit* s_pInstance;

	shared_ptr<CGameController> m_pGameInputCtrl;

	ViewList m_views;
	HUDList m_huds;


	// list of processes for background thread
	list<shared_ptr<IProcess>> m_processes;
	// mutex for m_processes
	IMutex* m_processesMutex;
	
};

inline shared_ptr<CGameController>
CGameCockpit::getGameController()
{
	return m_pGameInputCtrl;
}

#endif //_CGAMELOGIN_H_
