#include "CGameCockpit.h"
#include "CEventManager.h"

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

void CGameCockpit::run()
{
	// core of the application - logic 
	// read input from all parts of the system


	// Notify all controllers with related events - according to CEventManager rules
	CEventManager::instance()->notify();

	// prepares ORTHO PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //Reset the drawing perspective
	glOrtho(-(600.0 / 2.0), (600.0 / 2.0), -(600.0 / 2.0), (600.0 / 2.0), -1.0, 1.0);
	// prepares MODEL VIEW
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); 
	// Set background (clear) color to black
	glClearColor(0.0, 0.0, 0.0, 1.0); 
	//GLenum err = glGetError();
	//glMatrixMode(GL_PROJECTION);                // update projection
	//err = glGetError();

	// Render Screen objects
	for (ViewList::iterator it = m_views.begin();
			it != m_views.end();
			++it)
	{
		(*it)->VOnRender();
	}

	// Flush drawing command buffer to make drawing happen as soon as possible.
	glutSwapBuffers();
}

void CGameCockpit::ControllerMenuEventHandler(IEvent* ev)
{
	s_pInstance->m_CtrlMenu.execute(ev);
}

void CGameCockpit::setClass(string socketKey, ECharacterClass charClass)
{
	// remove it
}

CGameCockpit::CGameCockpit()
{
	CPerson* user1 = new CPerson("vagner", false, ECHARACTERGENDER_MALE, ECHARACTERRACE_HUMAN);
	CPerson* user2 = new CPerson("chen", false, ECHARACTERGENDER_FEMALE, ECHARACTERRACE_HUMAN);
	m_userDB.push_back(user1);
	m_userDB.push_back(user2);

	// creates an object of Human View
	shared_ptr<IViewElement> element(new CViewElementSquare(0, 0, 512, 512));
	shared_ptr<IViewElement> element2(new CViewElementSquare(-256, -256, 100, 100));
	shared_ptr<IView> view(new CHumanView());
	view->VPushElement(element);
	view->VPushElement(element2);
	//view->VPushElement(element3); 
	//view->VPushElement(element4);
	// push to the list of views
	m_views.push_back(view);
}

CGameCockpit::~CGameCockpit()
{
	while (!m_userDB.empty())
	{
		CPerson* it = m_userDB.back();
		m_userDB.pop_back();
		delete it;
		it = NULL;
	}


	while (!m_views.empty())
	{
		// shared pointers will automatically deallocate when the last reference is erased
		m_views.pop_back();
	}



	delete s_pInstance;
}
