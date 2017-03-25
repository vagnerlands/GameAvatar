#include "CCommandLoginGenderMenu.h"
#include "CEventManager.h"
#include "CEventMenu.h"

CCommandLoginGenderMenu::CCommandLoginGenderMenu(string menuOption)
{
	m_menuOption = menuOption;
}

void CCommandLoginGenderMenu::execute()
{
	ELoginGenderMenuOption eRes = validate(m_menuOption);

	CEventMenu* ev = NULL;
	ev = new CEventMenu();

	if (ev != NULL)
	{
		ev->SetKey(m_socket);
		ev->SetOption(static_cast<TInt32>(eRes));
		ev->SetState(ESOCKETSTATE_CREATE_NEW_CHAR_GENDER);
		CEventManager::instance()->addEvent(ev);
	}
}

ELoginGenderMenuOption CCommandLoginGenderMenu::validate(string menuOption)
{
	ELoginGenderMenuOption retVal = ELOGINGENDERMENUOPTION_SELECT_GENDER_INVALID;

	if ((menuOption == "M") || (menuOption == "m"))
	{
		retVal = ELOGINGENDERMENUOPTION_SELECT_MALE;
	}
	else if ((menuOption == "F") || (menuOption == "f"))
	{
		retVal = ELOGINGENDERMENUOPTION_SELECT_FEMALE;
	}
	else if ((menuOption == "r") || (menuOption == "R"))
	{
		retVal = ELOGINGENDERMENUOPTION_SELECT_RETURN_TO_MAIN_MENU;
	}

	return retVal;
}
