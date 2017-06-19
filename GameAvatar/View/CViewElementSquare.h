#ifndef _CVIEWELEMENTSQUARE_H_
#define _CVIEWELEMENTSQUARE_H_

#include "IViewElement.h"

class CViewElementSquare : public IViewElement
{
public:

	CViewElementSquare(TFloat posX, TFloat posY, TFloat width, TFloat height);

	virtual ~CViewElementSquare();

	virtual void VPreRender();

	virtual void VRender();

	// debug purposes
	virtual char* GetName()
	{
		return "Square";
	}
	virtual GameViewElement VGetType()
	{
		return GameViewElement_Square;
	}
protected:
	virtual void applyTexture(string textId);

};

#endif //_CVIEWELEMENTSQUARE_H_
