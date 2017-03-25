#ifndef _IVIEW_H_
#define _IVIEW_H_

#include "CCommonTypes.h"
#include "IViewElement.h"
#include <memory>


using namespace Types;
using namespace std;

class IView
{
public:
	// Implement the IGameView interface
	//explicit IView();
	virtual ~IView()
	{

	}
	virtual void VOnRender() = 0;
	virtual GameViewType VGetType() { return GameView_Undefined; }
	virtual GameViewId VGetId() const { return m_ViewId; }
	virtual void VPushElement(shared_ptr<IViewElement> pElement) = 0;
	virtual void VPopElement(shared_ptr<IViewElement> pElement) = 0;
protected:
	GameViewId m_ViewId;
	ViewElementList m_elements;
};


#endif
