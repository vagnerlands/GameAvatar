#ifndef _CEVENTMANAGER_H_
#define _CEVENTMANAGER_H_

#include "IEvent.h"
#include "CCommonTypes.h"
#include <string>
#include <unordered_map>
#include <list>

using namespace std;
using namespace Types;

//typedef list<TOnEvent>::iterator TEventListIterator;
typedef list<IEvent*> TEventList;
typedef void(*TOnEvent)(IEvent* ev);

class CEventManager {
public:
	static CEventManager* instance();
	void registerListener(EEventType type, TOnEvent callbackFunction);
	void addEvent(IEvent* ev);
	void notify();

private:
	static CEventManager* s_pInstance;
	CEventManager()
	{
		// no need to initialize...
	}

	~CEventManager();
	// list of registered functions per event type
	TOnEvent m_registered[EEventType::EEVENTTYPE_LAST];
	TEventList m_eventQ;


};

#endif
