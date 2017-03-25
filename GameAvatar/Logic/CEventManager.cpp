#include "CEventManager.h"

CEventManager* CEventManager::s_pInstance = NULL;

CEventManager* 
CEventManager::instance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CEventManager();
	}

	return s_pInstance;
}

void 
CEventManager::registerListener(EEventType type, TOnEvent callbackFunction)
{
	//CGameCockpit* callBack = static_cast<CGameCockpit*>(callbackFunction);
	m_registered[type] = callbackFunction;
}

void 
CEventManager::addEvent(IEvent * ev)
{
	m_eventQ.push_back(ev);
}

void 
CEventManager::notify()
{	
	while (!m_eventQ.empty())
	{	
		IEvent* lEvent = m_eventQ.front();
		m_eventQ.pop_front();
		// calls the respective registered Controller to treat the Event
		m_registered[lEvent->GetType()](lEvent);
	}
}

CEventManager::~CEventManager()
{
	delete s_pInstance;
}
