#ifndef _CHUMANVIEW_H_
#define _CHUMANVIEW_H_

#include "IView.h"

class CHumanView : public IView
{
public:
	CHumanView();
	virtual ~CHumanView();
	virtual void VOnRender();
	virtual void VPushElement(shared_ptr<IViewElement> pElement);
	virtual void VPopElement(shared_ptr<IViewElement> pElement);
};

#endif //_CHUMANVIEW_H_
