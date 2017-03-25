#include "CCommandLoginClassMenu.h"
#include "CEventManager.h"
#include "CEventMenu.h"

CCommandLoginClassMenu::CCommandLoginClassMenu(string menuOption)
{
	m_menuOption = menuOption;
}

void CCommandLoginClassMenu::execute()
{
	ELoginClassMenuOption eRes = validate(m_menuOption);

	CEventMenu* ev = NULL;
	ev = new CEventMenu();

	if (ev != NULL)
	{
		ev->SetKey(m_socket);
		ev->SetOption(static_cast<TInt32>(eRes));
		ev->SetState(ESOCKETSTATE_CREATE_NEW_CHAR_CLASS);
		CEventManager::instance()->addEvent(ev);
	}
}

ELoginClassMenuOption 
CCommandLoginClassMenu::validate(string menuOption)
{
	ELoginClassMenuOption retVal = ELOGINCLASSMENUOPTION_SELECT_INVALID;
	if (menuOption == "1")
	{
		retVal = ELOGINCLASSMENUOPTION_SELECT_WARRIOR;
	}
	else if (menuOption == "2")
	{
		retVal = ELOGINCLASSMENUOPTION_SELECT_MAGE;
	}
	else if (menuOption == "3")
	{
		retVal = ELOGINCLASSMENUOPTION_SELECT_PALADIN;
	}
	else if (menuOption == "4")
	{
		retVal = ELOGINCLASSMENUOPTION_SELECT_NINJA;
	}
	else if ((menuOption == "r") || (menuOption == "R"))
	{
		retVal = ELOGINCLASSMENUOPTION_SELECT_RETURN_TO_MAIN_MENU;
	}
	return retVal;
}
