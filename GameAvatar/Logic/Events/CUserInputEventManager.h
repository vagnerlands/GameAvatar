#ifndef _CUSERINPUTEVENTMANAGER_H_
#define _CUSERINPUTEVENTMANAGER_H_

//#include "IUserInputListener.h"
#include "CCommonTypes.h"
#include <string>
#include <unordered_map>
#include <vector>

class IUserInputListener;

using namespace std;
using namespace Types;

struct SBindProperties
{
	SBindProperties()
	{
		m_key = 0;
		m_status = 0;
	}
	SBindProperties(IUserInputListener* pUserReference, Byte key, Int32 status)
		: m_pUserReference(pUserReference), m_key(key), m_status(status)
	{
		// empty implementation
	}
	// user reference
	IUserInputListener* m_pUserReference;
	// key value - ascii
	Byte m_key;
	// 0 for pressed
	// 1 for released
	Int32 m_status;
};

typedef unordered_map<Byte, vector<SBindProperties>> UserInputMap;

class CUserInputEventManager {
public:
	static CUserInputEventManager* instance();
	// register a key binding to a user listener
	void RegisterListener(IUserInputListener* user, Byte key, Int32 status);
	// notify all registered users about the key pressed - for treatment
	void NotifyEvent(Byte keyId, Int32 keyStatus);

private:
	// singleton object
	static CUserInputEventManager* s_pInstance;
	// private constructor - accessible by instance() method
	CUserInputEventManager()
	{
		// no need to initialize...
	}
	// destructor
	~CUserInputEventManager();
	// DS for binded keys
	// for each registered key binding, there will be a list of listeners to be triggered
	UserInputMap m_keyBindings;

};

#endif //_CUSERINPUTEVENTMANAGER_H_
