#ifndef _IKEYBOARDHANDLER_H_
#define _IKEYBOARDHANDLER_H_

#include "CCommonTypes.h"
#include <string>

using namespace std;
using namespace Types;

class IKeyboardHandler {
public:
	virtual ~IKeyboardHandler()
	{
	}

	virtual bool VOnKeyDown(const TByte c) = 0;

	virtual bool VOnKeyUp(const TByte c) = 0;


	virtual bool VOnMouseMove(const CPoint& mousePos) = 0;
	virtual bool VOnLButtonDown(const CPoint& mousePos) = 0;
	virtual bool VOnLButtonUp(const CPoint& mousePos) = 0;
	virtual bool VOnRButtonDown(const CPoint& mousePos) = 0;
	virtual bool VOnRButtonUp(const CPoint& mousePos) = 0;

	// Which keys are up and down
	bool m_bKey[256];
};

#endif //_IKEYBOARDHANDLER_H_
