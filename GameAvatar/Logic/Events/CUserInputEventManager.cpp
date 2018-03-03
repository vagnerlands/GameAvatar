#include "CUserInputEventManager.h"
#include "IUserInputListener.h"

CUserInputEventManager* CUserInputEventManager::s_pInstance = nullptr;


CUserInputEventManager::~CUserInputEventManager()
{
	delete s_pInstance;
}

CUserInputEventManager* CUserInputEventManager::instance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new CUserInputEventManager();
	}

	return s_pInstance;
}

void CUserInputEventManager::RegisterListener(IUserInputListener* user, Byte key, Int32 status)
{
	// iterator - ready for update in case node is found
	UserInputMap::iterator iteratorInMap = m_keyBindings.find(key);
	// if item found...
	if (iteratorInMap != m_keyBindings.end())
	{
		// add this user to this event - WATCH OUT that this method
		// is not checking if user was already there
		iteratorInMap->second.push_back(SBindProperties(user, key, status));
	}
	else
	{
		// if not found - must add it to the list		
		vector<SBindProperties> listOfValues;
		// adds user to the this list
		listOfValues.push_back(SBindProperties(user, key, status));
		// hopefully - the list above will be copied (only 1 item, not big deal) to the dictionary list
		m_keyBindings.insert({ key, listOfValues });
	}
}

void CUserInputEventManager::NotifyEvent(Byte keyId, Int32 keyStatus)
{
	// try to find a binded key
	UserInputMap::const_iterator readOnlyIterator = m_keyBindings.find(keyId);
	// checks for validity
	if (readOnlyIterator != m_keyBindings.end())
	{
		// iterates through all binded users
		for (Int32 listIndex = 0; listIndex < readOnlyIterator->second.size(); listIndex++)
		{
			// check is the registered event is for "pressed" or "released"
			if (readOnlyIterator->second[listIndex].m_status == keyStatus)
			{
				// triggers the event in "users" space
				readOnlyIterator->second[listIndex].m_pUserReference->VProcessUserInput(keyId, keyStatus);
			}
		}
	}

}
