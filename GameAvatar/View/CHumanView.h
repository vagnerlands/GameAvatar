#ifndef _CHUMANVIEW_H_
#define _CHUMANVIEW_H_

#include "IView.h"
#include "GL/glut.h"

class CHumanView : public IView
{
public:
	CHumanView();
	virtual ~CHumanView();
	virtual void VOnRender();
	virtual void VPushElement(const string elementId, shared_ptr<IViewElement> pElement);
	virtual void VPopElement(const string elementId, shared_ptr<IViewElement> pElement);
	virtual void VSetCamera(CCamera* pCamera);

private:
	static const TFloat s_DEFAULT_MOVEMENT_DISTANCE;
	void UpdateScenario(const string elementId);
	void UpdateCameraLocation();
};

inline void 
CHumanView::VSetCamera(CCamera * pCamera)
{
	m_pCamera = pCamera;
}

#endif //_CHUMANVIEW_H_
