#include "CGameCockpit.h"
#include "CEventManager.h"
#ifdef _WIN_
#include "CWinMutex.h"
#endif

#include <time.h>

CGameCockpit* CGameCockpit::s_pInstance = NULL;

CGameCockpit* 
CGameCockpit::instance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CGameCockpit();
	}
	return s_pInstance;
}

/*
bool 
CGameCockpit::validateUserLogin(string socketKey, string login, bool* isCharacterCreated)
{
	bool retVal = false;
	for (list<CPerson*>::iterator it = m_userDB.begin(); it != m_userDB.end(); it++)
	{		
		if ((*it)->getUser() == login)
		{
			cout << "Key [" << socketKey << "] typed " << login << " and it's valid!" << endl;
			retVal = true;
			*isCharacterCreated = (*it)->isCharacterCreated();
		}
	}

	if (!retVal)
		cout << "Key [" << socketKey << "] typed " << login << ", this user is not valid!" << endl;
	
	return retVal;
}
*/

void CGameCockpit::run()
{

	// limit fps to 60
	clock_t start = clock();

	// prepares ORTHO PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //Reset the drawing perspective
					  //glOrtho(-(s_SCREEN_WIDTH / 2.0), (s_SCREEN_WIDTH / 2.0), -(s_SCREEN_HEIGHT / 2.0), (s_SCREEN_HEIGHT / 2.0), -600.0, 600.0);
					  //glFrustum(-(s_SCREEN_WIDTH / 2.0), (s_SCREEN_WIDTH / 2.0), -(s_SCREEN_HEIGHT / 2.0), (s_SCREEN_HEIGHT / 2.0), 0.1f, 60000.0f);
	static GLfloat frustumParams[] = { -1.f, 1.f, -1.f, 1.f, 5.f, 10000.f };
	glFrustum(frustumParams[0],
		frustumParams[1],
		frustumParams[2],
		frustumParams[3],
		frustumParams[4],
		frustumParams[5]);

	m_camera.SetLookAtMatrix();

	// prepares MODEL VIEW
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// Set background (clear) color to black
	glClearColor(0.0, 0.0, 0.0, 1.0);


	// Notify all controllers with related events - according to CEventManager rules
	CEventManager::instance()->notify();

	// Render Screen objects
	for (ViewList::iterator it = m_views.begin();
			it != m_views.end();
			++it)
	{
		(*it)->VOnRender();
	}

	while (clock() - start < Types::s_CYCLE_MAX_TIME)
	{
		// no operation
	}

}

void CGameCockpit::ControllerMenuEventHandler(IEvent* ev)
{
	s_pInstance->m_CtrlMenu.execute(ev);
}

void CGameCockpit::UserInputEventHandler(IEvent* ev)
{
	s_pInstance->m_CtrlMenu.execute(ev);
}


void CGameCockpit::pushProcess(shared_ptr<IProcess> pProcess)
{
	m_processesMutex->mutexLock();
	for (list<shared_ptr<IProcess>>::iterator it = m_processes.begin();
		it != m_processes.end();
		++it)
	{
		shared_ptr<IProcess> r = *it;
		if (r->VGetType() == pProcess->VGetType())
		{
			m_processesMutex->mutexUnlock();
			// same process already exists in the queue - then do not add a new one
			return;
		}
	}

	// adds a new process in the queue
	m_processes.push_front(pProcess);
	m_processesMutex->mutexUnlock();
}

shared_ptr<IProcess> CGameCockpit::getNextProcess()
{
	shared_ptr<IProcess> retVal = NULL;
	m_processesMutex->mutexLock();
	if (!m_processes.empty())
	{
		retVal = m_processes.back();
	}
	m_processesMutex->mutexUnlock();
	return retVal;
}

void 
CGameCockpit::popProcess(shared_ptr<IProcess> pProcess)
{
	m_processesMutex->mutexLock();
	for (list<shared_ptr<IProcess>>::iterator it = m_processes.begin();
		m_processes.end() != it;
		++it)
	{
		shared_ptr<IProcess> r = *it;
		if (pProcess == r)
		{
			m_processes.erase(it);
			break;
		}
	}
	m_processesMutex->mutexUnlock();
}

CGameCockpit::CGameCockpit()
{
#ifdef _WIN_
	m_processesMutex = new CWinMutex();
#else
	#error "no implementation for this platform"
#endif
	if (m_processesMutex == NULL)
	{
		cout << "<!> Critical issue while allocating resources, program is being terminated!" << endl;
		exit(1);
	}
	// create a mutex for background processes of this game
	m_processesMutex->createMutex("backgroundProcesses");
	// creates the game control
	shared_ptr<CGameController> pGameControl(new CGameController());
	m_pGameInputCtrl = pGameControl;
	// set the game control to the current view
	shared_ptr<IView> view(new CHumanView());
	view->VSetGameCtrl(m_pGameInputCtrl);

	// creates an object of Human View
	shared_ptr<IViewElement> element(new CViewElementSquare(0.0f, 0.0f, 512.0f, 512.0f, "melancia.bmp"));
	shared_ptr<IViewElement> element2(new CViewElementSquare(-256.0f, -256.0f, 100.0f, 100.0f, "melancia.bmp"));
	shared_ptr<IViewElement> skull(new CViewElementModel(-5.0f, 0.0f, 2.0f, 2.0f, 2.0f, "skull.obj"));
	shared_ptr<IViewElement> panda(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "panda.obj"));
	shared_ptr<IViewElement> tiger(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "tiger.obj"));
	shared_ptr<IViewElement> master(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "master.obj"));
	shared_ptr<IViewElement> monkey(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "monky.obj"));
	//shared_ptr<IViewElement> bread(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "ford.obj"));
	//shared_ptr<IViewElement> elementCube2(new CViewElementModel(-150.0f, 0.0f, 2.0f, 2.0f, 2.0f, "skull.obj"));
	//shared_ptr<IViewElement> elementCube3(new CViewElementModel(100.0f, 0.0f, 2.0f, 2.0f, 2.0f, "skull.obj"));

	//view->VPushElement("melan", element);
	//view->VPushElement(element2);
	view->VPushElement("skull", skull);
	view->VPushElement("panda", panda);
	//view->VPushElement("monkey", monkey);
	//view->VPushElement("tiger", tiger);
	//view->VPushElement("master", master);
	//view->VPushElement("bread", bread);
	//view->VPushElement("skull2", elementCube2);
	//view->VPushElement("skull3", elementCube3);
	//view->VPushElement(element3); 
	//view->VPushElement(element4);
	// push to the list of views
	m_views.push_back(view);

	m_camera.SetCameraAttribute(CameraAttributeType::CameraAttribute_Position, 0.0f, 0.0f, 50.f);
	m_camera.SetCameraAttribute(CameraAttributeType::CameraAttribute_Up, 0.0f, 1.0f, 0.f);
	m_camera.SetCameraAttribute(CameraAttributeType::CameraAttribute_Right, 1.0f, 0.0f, 0.f);
	m_camera.SetCameraAttribute(CameraAttributeType::CameraAttribute_Forward, 0.0f, 0.0f, -1.0f);
}

void
CGameCockpit::Close()
{

	while (!m_views.empty())
	{
		// shared pointers will automatically deallocate when the last reference is erased
		m_views.pop_back();
	}

	while (!m_processes.empty())
	{
		// shared pointers will automatically deallocate when the last reference is erased
		m_processes.pop_back();
	}

	m_processesMutex->mutexUnlock();
	m_processesMutex->destroy();

	delete m_processesMutex;

	delete s_pInstance;

}

CGameCockpit::~CGameCockpit()
{
	while (!m_views.empty())
	{
		// shared pointers will automatically deallocate when the last reference is erased
		m_views.pop_back();
	}

	s_pInstance = nullptr;
}
