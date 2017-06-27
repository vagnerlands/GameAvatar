#ifndef _CBASEUI_H_
#define _CBASEUI_H_

#include "CCommonTypes.h"
#include "IView.h"
#include <string>

using namespace std;
using namespace Types;

class CBaseUI 
{
public:

protected:
	shared_ptr<IView> m_pView;



};

#endif //_CBASEUI_H_
