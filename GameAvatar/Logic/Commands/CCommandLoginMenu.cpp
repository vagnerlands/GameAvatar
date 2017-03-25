#include "CCommandLoginMenu.h"
#include "CEventManager.h"
#include "CEventMenu.h"

CCommandLoginMenu::CCommandLoginMenu(string menuOption)
{
	m_menuOption = menuOption;
}

void CCommandLoginMenu::execute()
{
	ELoginMenuOption eRes = validate(m_menuOption);
	CEventMenu* ev = NULL;
	ev = new CEventMenu();
	
	if (ev != NULL)
	{
		ev->SetOption(static_cast<TInt32>(eRes));
		ev->SetKey(m_socket);
		ev->SetState(ESOCKETSTATE_LOGIN_MENU);
		CEventManager::instance()->addEvent(ev);
	}

	//if (m_socket != NULL)
	//{
	//	m_socket->write(message.getData().c_str(), message.getData().length());
	//}
}

ELoginMenuOption
CCommandLoginMenu::validate(string menuOption)
{
	ELoginMenuOption retVal = ELOGINMENUOPTION_INVALID;

	if (menuOption == "1")
	{
		retVal = ELOGINMENUOPTION_CREATE_NEW_CHARACTER;
	}
	else if (menuOption == "2")
	{
		retVal = ELOGINMENUOPTION_START_GAME;
	}
	else if (menuOption == "3")
	{
		retVal = ELOGINMENUOPTION_LEAVE_GAME;
	}

	return retVal;
}
