#ifndef _CGAMECONTROLLER_H_
#define _CGAMECONTROLLER_H_

#include "CCommonTypes.h"
#include "IKeyboardHandler.h"
#include "IMouseHandler.h"
#include "CUserInputEventManager.h"
#include <string>

using namespace std;
using namespace Types;

class CGameController : public IKeyboardHandler, public IMouseHandler
{

public:

	// constructor
	CGameController();
	// destructor
	virtual ~CGameController();

	// this function is called cyclically and this updates the game status...
	void OnUpdate();

	virtual bool VOnMouseMove(const CPoint &mousePos);

	virtual bool VOnLButtonDown(const CPoint &mousePos)
	{
		return true;
	}

	virtual bool VOnLButtonUp(const CPoint &mousePos)
	{
		return true;
	}

	virtual bool VOnRButtonDown(const CPoint &mousePos)
	{
		return true;
	}

	virtual bool VOnRButtonUp(const CPoint &mousePos) 
	{
		return true;
	}

	bool VOnKeyDown(const Byte c) 
	{ 
		m_bKey[c] = true; 
		return true; 
	}

	bool VOnKeyUp(const Byte c) 
	{ 
		m_bKey[c] = false; 
		// notify that key "c" was released
		CUserInputEventManager::instance()->NotifyEvent(c, EKeyStatus::KeyStatus_Released);
		return true; 
	}

};

#endif //_IEVENT_H_
