#include "CCommandLoginRaceMenu.h"
#include "CEventManager.h"
#include "CEventMenu.h"

CCommandLoginRaceMenu::CCommandLoginRaceMenu(string menuOption)
{
	m_menuOption = menuOption;
}

void CCommandLoginRaceMenu::execute()
{
	ELoginRaceMenuOption eRes = validate(m_menuOption);

	CEventMenu* ev = NULL;
	ev = new CEventMenu();

	if (ev != NULL)
	{
		ev->SetKey(m_socket);
		ev->SetOption(static_cast<TInt32>(eRes));
		ev->SetState(ESOCKETSTATE_CREATE_NEW_CHAR_RACE);
		CEventManager::instance()->addEvent(ev);
	}
}

ELoginRaceMenuOption 
CCommandLoginRaceMenu::validate(string menuOption)
{
	ELoginRaceMenuOption retVal = ELOGINRACEMENUOPTION_SELECT_INVALID;
	if (menuOption == "1")
	{
		retVal = ELOGINRACEMENUOPTION_SELECT_HUMAN;
	}
	else if (menuOption == "2")
	{
		retVal = ELOGINRACEMENUOPTION_SELECT_ELF;
	}
	else if (menuOption == "3")
	{
		retVal = ELOGINRACEMENUOPTION_SELECT_ORC;
	}
	else if (menuOption == "4")
	{
		retVal = ELOGINRACEMENUOPTION_SELECT_DWARF;
	}
	else if ((menuOption == "r") || (menuOption == "R"))
	{
		retVal = ELOGINRACEMENUOPTION_SELECT_RETURN_TO_MAIN_MENU;
	}
	return retVal;

}
