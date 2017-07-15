#ifndef _IVIEW_H_
#define _IVIEW_H_

#include "CCommonTypes.h"
#include "IViewElement.h"
#include "CGameController.h"
#include "CCamera.h"
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
	virtual void VSetGameCtrl(shared_ptr<CGameController> pGameCtrl) { m_pGameCtrl = pGameCtrl; }
	virtual void VPushElement(const string elementId, shared_ptr<IViewElement> pElement) = 0;
	virtual void VPopElement(const string elementId, shared_ptr<IViewElement> pElement) = 0;
	virtual void VSetCamera(CCamera* pCamera) = 0;
protected:
	GameViewId m_ViewId;
	ViewElementMap m_elements;
	ViewLightMap m_lightSources;
	shared_ptr<CGameController> m_pGameCtrl;
	CCamera* m_pCamera;
};


#endif
