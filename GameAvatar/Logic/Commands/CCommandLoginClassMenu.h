#ifndef _COMMANDLOGINCLASSMENU_H_
#define _COMMANDLOGINCLASSMENU_H_

#include "CICommand.h"
#include "CGameCockpit.h"
#include "CCommonTypes.h"
#include <string>

using namespace std;
using namespace Types;

class CCommandLoginClassMenu : public CICommand {
public:
	CCommandLoginClassMenu(string menuOption);
	void execute();
	ELoginClassMenuOption validate(string menuOption);
private:
	//CGameCockpit *m_object;
	//Action m_method;
	string m_menuOption;
};

#endif
