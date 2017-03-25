#ifndef _CCOMMANDLOGINRACEMENU_H_
#define _CCOMMANDLOGINRACEMENU_H_

#include "CICommand.h"
#include "CCommonTypes.h"
#include "CGameCockpit.h"
#include <string>

using namespace std;
using namespace Types;

class CCommandLoginRaceMenu : public CICommand {
public:
	CCommandLoginRaceMenu(string menuOption);
	void execute();
	ELoginRaceMenuOption validate(string menuOption);
private:
	//CGameCockpit *m_object;
	//Action m_method;
	string m_menuOption;
};

#endif
