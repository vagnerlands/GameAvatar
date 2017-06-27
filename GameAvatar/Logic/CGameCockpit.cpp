#include "CGameCockpit.h"
#include "CEventManager.h"
#ifdef _WIN_
#include "CWinMutex.h"
#endif

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
	// core of the application - logic 
	// read input from all parts of the system


	// Notify all controllers with related events - according to CEventManager rules
	CEventManager::instance()->notify();

	// Render Screen objects
	for (ViewList::iterator it = m_views.begin();
			it != m_views.end();
			++it)
	{
		(*it)->VOnRender();
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
	shared_ptr<IViewElement> elementCube(new CViewElementModel(0.0f, 0.0f, 2.0f, 2.0f, 2.0f, "skull.obj"));

	//view->VPushElement("melan", element);
	//view->VPushElement(element2);
	view->VPushElement("skull", elementCube);
	//view->VPushElement(element3); 
	//view->VPushElement(element4);
	// push to the list of views
	m_views.push_back(view);
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
