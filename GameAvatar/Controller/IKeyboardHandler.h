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

	virtual bool VOnKeyDown(const Byte c) = 0;

	virtual bool VOnKeyUp(const Byte c) = 0;


	virtual bool VOnMouseMove(const CPoint& mousePos) = 0;
	virtual bool VOnLButtonDown(const CPoint& mousePos) = 0;
	virtual bool VOnLButtonUp(const CPoint& mousePos) = 0;
	virtual bool VOnRButtonDown(const CPoint& mousePos) = 0;
	virtual bool VOnRButtonUp(const CPoint& mousePos) = 0;

	// last key status
	bool m_bKeyStatus[256];
	// transitory status
	// once per cycle, these keys are copied to m_bKeyStatus
	// this is used to detect "on release" events
	bool m_bKey[256];

};

#endif //_IKEYBOARDHANDLER_H_
