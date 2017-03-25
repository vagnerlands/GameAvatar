#ifndef _IEVENT_H_
#define _IEVENT_H_

#include "CCommonTypes.h"
#include <string>

using namespace std;
using namespace Types;

class IEvent {
public:
	//virtual ~IEvent() = 0;

	// debug only
	virtual string GetName()
	{
		// debug only
		return m_eventName;
	}

	virtual EEventType GetType()
	{
		// illegal value - avoid memory violation
		return EEventType::EEVENTTYPE_ILLEGAL;
	}

	// attributes and other information

protected:
	// shall have a pointer to the room location where 
	string m_eventName;
};

#endif //_IEVENT_H_
