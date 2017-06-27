#include "CControllerMenu.h"
#include "CEventMenu.h"
#include "CAnsiString.h"

CControllerMenu::CControllerMenu()
	: m_socketKey("")
{
	// empty implementation
}

string
CControllerMenu::buildMenu(ESocketState nextState)
{
	// return string
	CAnsiString retStr = "";
	retStr.resetFormat();
	retStr += "\r\n";
	retStr.setForegroundColor(Types::EANSICOLOR_CYAN);

	switch (nextState)
	{
	case ESOCKETSTATE_LOGIN_MENU:
		retStr += " MUD Avatar Menu:\r\n";
		retStr += "-----------------\r\n";
		retStr += "\r\n [1] Create new character";
		retStr += "\r\n [2] Start GAME";
		retStr += "\r\n [3] Leave the game";
		retStr += "\r\n> Choose: ";
		retStr.setBackgroundColor(Types::EANSICOLOR_RED);
		break;
	case ESOCKETSTATE_CREATE_NEW_CHAR_GENDER:
		retStr += "\r\nWhat is your gender: ";
		retStr += "\r\n [M] Male ";
		retStr += "\r\n [F] Female ";
		retStr += "\r\n [r] Return to Main Menu \r\n ";
		retStr += "\r\n > Choose: ";
		retStr.setBackgroundColor(Types::EANSICOLOR_RED);
		break;
	case ESOCKETSTATE_CREATE_NEW_CHAR_CLASS:
		retStr += "\r\nSelect one of the available classes:";
		retStr += "\r\n[1] Warrior";
		retStr += "\r\n[2] Mage";
		retStr += "\r\n[3] Paladin";
		retStr += "\r\n[4] Ninja";
		retStr += "\r\n[r] Return to Main Menu \r\n";
		retStr += "\r\n> Choose: ";
		retStr.setBackgroundColor(Types::EANSICOLOR_RED);
		break;
	case ESOCKETSTATE_CREATE_NEW_CHAR_RACE:
		retStr += "\r\nWhat will be your race: ";
		retStr += "\r\n [1] Human ";
		retStr += "\r\n [2] Elf ";
		retStr += "\r\n [3] Orc ";
		retStr += "\r\n [4] Dwarf ";
		retStr += "\r\n [r] Return to Main Menu \r\n";
		retStr += "\r\n> Choose: ";
		retStr.setBackgroundColor(Types::EANSICOLOR_RED);
		break;
	case ESOCKETSTATE_IN_GAME:
		retStr.setBold();
		retStr.setForegroundColor(Types::EANSICOLOR_YELLOW);
		retStr += "\r\nThe game will start now...\r\n";
		break;
	default:
		retStr = "unknown option";
		break;
	}

	return retStr.getData();
}

void 
CControllerMenu::execute(IEvent* ev)
{
	// if the code got to here, we know for sure that this type is an "CEventMenu" type
	CEventMenu* downcasted = dynamic_cast<CEventMenu*>(ev);
	CAnsiString message;
	ESocketState nextState;
	message.resetFormat();
	message += "\r\n";
	message.setForegroundColor(Types::EANSICOLOR_CYAN);

	if (ESOCKETSTATE_LOGIN == downcasted->GetState())
	{
		bool bRes = static_cast<bool>(downcasted->GetOption());
		if (bRes)
		{
			message += "\r\n> Login accepted!\r\n";
			nextState = ESOCKETSTATE_LOGIN_MENU;
		}
		else
		{
			message += "\r\n> Non existing user\r\n";
			message += "\r\n> Please, insert your user: ";
			message.setBackgroundColor(EANSICOLOR_RED);
			nextState = ESOCKETSTATE_LOGIN;
		}
	}
	else if (ESOCKETSTATE_LOGIN_MENU == downcasted->GetState())
	{
		switch (static_cast<ELoginMenuOption>(downcasted->GetOption()))
		{
		case ELOGINMENUOPTION_CREATE_NEW_CHARACTER:
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_CLASS;
			break;
		case ELOGINMENUOPTION_START_GAME:
			message += "Entering the game...\r\n\r\n";
			nextState = ESOCKETSTATE_IN_GAME;
			break;
		case ELOGINMENUOPTION_LEAVE_GAME:
			message += "Goodbye...\r\n";
			downcasted->GetKey()->write(message.getData().c_str(), message.getData().length());
			downcasted->GetKey()->recycle();
			break;
		default:
			message += "Option is not available!\r\n";
			nextState = ESOCKETSTATE_LOGIN_MENU;
			break;
		}
	}
	else if (ESOCKETSTATE_CREATE_NEW_CHAR_CLASS == downcasted->GetState())
	{
		switch (static_cast<ELoginClassMenuOption>(downcasted->GetOption()))
		{
		case ELOGINCLASSMENUOPTION_SELECT_WARRIOR:
			message += "\r\nYou're now a Warrior!";
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_GENDER;
			break;
		case ELOGINCLASSMENUOPTION_SELECT_MAGE:
			message += "\r\nYou're now a Mage!";
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_GENDER;
			break;
		case ELOGINCLASSMENUOPTION_SELECT_PALADIN:
			message += "\r\nYou're now a Paladin!";
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_GENDER;
			break;
		case ELOGINCLASSMENUOPTION_SELECT_NINJA:
			message += "\r\nYou're now a Ninja!";
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_GENDER;
			break;
		case ELOGINCLASSMENUOPTION_SELECT_RETURN_TO_MAIN_MENU:
			message += "\r\n...\r\n";
			nextState = ESOCKETSTATE_LOGIN_MENU;
			break;
		default:
			message += "Option is not available!\r\n";
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_CLASS;
			break;
		}
	}
	else if (ESOCKETSTATE_CREATE_NEW_CHAR_RACE == downcasted->GetState())
	{
		switch (static_cast<ELoginRaceMenuOption>(downcasted->GetOption()))
		{
		case ELOGINRACEMENUOPTION_SELECT_HUMAN:
			message += "\r\nYou're now a Human!";
			nextState = ESOCKETSTATE_IN_GAME;
			break;
		case ELOGINRACEMENUOPTION_SELECT_ELF:
			message += "\r\nYou're now an Elf!";
			nextState = ESOCKETSTATE_IN_GAME;
			break;
		case ELOGINRACEMENUOPTION_SELECT_ORC:
			message += "\r\nYou're now an Orc!";
			nextState = ESOCKETSTATE_IN_GAME;
			break;
		case ELOGINRACEMENUOPTION_SELECT_DWARF:
			message += "\r\nYou're now a Dwarf!";
			nextState = ESOCKETSTATE_IN_GAME;
			break;
		case ELOGINRACEMENUOPTION_SELECT_RETURN_TO_MAIN_MENU:
			message += "\r\n...\r\n";
			nextState = ESOCKETSTATE_LOGIN_MENU;
			break;
		default:
			message += "Option is not available!\r\n";
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_RACE;
			break;
		}
	}
	else if (ESOCKETSTATE_CREATE_NEW_CHAR_GENDER == downcasted->GetState())
	{
		switch (static_cast<ELoginGenderMenuOption>(downcasted->GetOption()))
		{
		case ELOGINGENDERMENUOPTION_SELECT_FEMALE:
			message += "\r\nOk Milady!";
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_RACE;
			break;
		case ELOGINGENDERMENUOPTION_SELECT_MALE:
			message += "\r\nOk Sir!";
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_RACE;
			break;
		case ELOGINGENDERMENUOPTION_SELECT_RETURN_TO_MAIN_MENU:
			message += "\r\n...\r\n";
			nextState = ESOCKETSTATE_LOGIN_MENU;
			break;
		default:
			message += "Option is not available!\r\n";
			nextState = ESOCKETSTATE_CREATE_NEW_CHAR_GENDER;
			break;
		}
	}

	downcasted->GetKey()->setState(nextState);

	message += buildMenu(nextState);

	downcasted->GetKey()->write(message.getData().c_str(), message.getData().length());

	// finally - releases the IEvent pointer object
	delete ev;
	ev = NULL;
}