#ifndef _CCONTROLLERMENU_H_
#define _CCONTROLLERMENU_H_

#include "CCommonTypes.h"
#include "IEvent.h"
#include <string>

using namespace std;
using namespace Types;

class CControllerMenu {
public:

	CControllerMenu();

	virtual void execute(IEvent* ev);

	virtual string GetKey()
	{
		return m_socketKey;
	}
	
	// attributes and other information

protected:
	string buildMenu(ESocketState nextState);
	string m_socketKey;

};

#endif //_CCONTROLLERMENU_H_
