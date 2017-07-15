#ifndef _IMOUSEHANDLER_H_
#define _IMOUSEHANDLER_H_

#include "CCommonTypes.h"
#include <string>

using namespace std;
using namespace Types;

class IMouseHandler
{
public:

	virtual ~IMouseHandler()
	{

	}
	
	virtual bool VOnMouseMove(const CPoint& mousePos) = 0;
	virtual bool VOnLButtonDown(const CPoint& mousePos) = 0;
	virtual bool VOnLButtonUp(const CPoint& mousePos) = 0;
	virtual bool VOnRButtonDown(const CPoint& mousePos) = 0;
	virtual bool VOnRButtonUp(const CPoint& mousePos) = 0;
	
	// attributes about mouse movement for decisions
	TInt32 m_movementIntensityX;
	TInt32 m_movementIntensityY;
	TInt32 m_movementAngle;
	TInt32 m_distanceFromCenter;
	TInt32 m_angleFromCenter;

	CPoint m_lastPos;
	
};

#endif //_IMOUSEHANDLER_H_
