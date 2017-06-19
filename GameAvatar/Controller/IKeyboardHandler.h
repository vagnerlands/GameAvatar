#ifndef _IKEYBOARDHANDLER_H_
#define _IKEYBOARDHANDLER_H_

#include "CCommonTypes.h"
#include <string>

using namespace std;
using namespace Types;

class IKeyboardHandler {
public:
	virtual ~IKeyboardHandler() = 0;

	virtual bool VOnKeyDown(const TByte c) = 0;

	virtual bool VOnKeyUp(const TByte c) = 0;
};

#endif //_IKEYBOARDHANDLER_H_
