#ifndef _COMMANDLOGINMENU_H_
#define _COMMANDLOGINMENU_H_

#include "CICommand.h"
#include "CGameCockpit.h"
#include "CCommonTypes.h"
#include <string>

using namespace std;
using namespace Types;

class CCommandLoginMenu : public CICommand {
public:
	CCommandLoginMenu(string menuOption);
	void execute();
	ELoginMenuOption validate(string menuOption);
private:
	//CGameCockpit *m_object;
	//Action m_method;
	string m_menuOption;
};

#endif
