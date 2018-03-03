#ifndef _IUSERINPUTLISTENER_H_
#define _IUSERINPUTLISTENER_H_

#include "CCommonTypes.h"
#include <string>

using namespace std;
using namespace Types;

class IUserInputListener {
public:
	//virtual ~IUserInputListener() = 0;

	// debug only
	virtual string VGetName()
	{
		// debug only
		return m_userInputListenerName;
	}

	// pure virtual - user input
	// this shall be implemented by IView type
	virtual void VProcessUserInput(Byte keyId, Int32 keyStatus) = 0;

protected:
	// the derivate class shall assign a valid name here
	string m_userInputListenerName;
};

#endif //_IEVENT_H_
