#include "CCommandLoginUserName.h"
#include "CEventManager.h"
#include "CEventMenu.h"

CCommandLoginUserName::CCommandLoginUserName(string login)
{
	m_login = login;
}

void CCommandLoginUserName::execute()
{
	// the result of this value will be used to decide the menu content
	bool isCharacterCreated = false;
	// validates given user
	bool bRes = CGameCockpit::instance()->validateUserLogin(m_socketKey, m_login, &isCharacterCreated);
	
	CEventMenu* ev = NULL;
	ev = new CEventMenu();

	if (ev != NULL)
	{
		ev->SetKey(m_socket);
		ev->SetOption(static_cast<TInt32>(bRes));
		ev->SetState(ESOCKETSTATE_LOGIN);
		CEventManager::instance()->addEvent(ev);
	}
}
