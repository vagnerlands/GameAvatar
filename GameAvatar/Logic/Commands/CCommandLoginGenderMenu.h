#ifndef _CCOMMANDLOGINGENDERMENU_H_
#define _CCOMMANDLOGINGENDERMENU_H_

#include "CICommand.h"
#include "CGameCockpit.h"
#include "CCommonTypes.h"
#include <string>

using namespace std;
using namespace Types;

class CCommandLoginGenderMenu : public CICommand {
public:
	CCommandLoginGenderMenu(string menuOption);
	void execute();
	ELoginGenderMenuOption validate(string menuOption);
private:
	//CGameCockpit *m_object;
	//Action m_method;
	string m_menuOption;
};

#endif
