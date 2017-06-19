#ifndef _CGAMECONTROLLER_H_
#define _CGAMECONTROLLER_H_

#include "CCommonTypes.h"
#include "IKeyboardHandler.h"
#include <string>

using namespace std;
using namespace Types;

class CGameController : public IKeyboardHandler
{
protected:
	// Which keys are up and down
	TByte m_bKey[256]; 
	//shared_ptr<SceneNode>	m_object;
public:
	// this function is called cyclically and this updates the game status...
	void OnUpdate();

	bool VOnKeyDown(const TByte c) 
	{ 
		m_bKey[c] = true; 
		return true; 
	}

	bool VOnKeyUp(const TByte c) 
	{ 
		m_bKey[c] = false; 
		return true; 
	}

};

#endif //_IEVENT_H_
