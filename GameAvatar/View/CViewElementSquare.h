#ifndef _CVIEWELEMENTSQUARE_H_
#define _CVIEWELEMENTSQUARE_H_

#include "IViewElement.h"

class CViewElementSquare : public IViewElement
{
public:

	CViewElementSquare(TInt32 posX, TInt32 posY, TInt32 width, TInt32 height);

	virtual ~CViewElementSquare();

	virtual void VPreRender();

	virtual void VRender();

	// debug purposes
	virtual char* GetName()
	{
		return "SquareElement";
	}
	virtual GameViewElement VGetType()
	{
		return GameViewElement_Square;
	}
protected:
	virtual void bindTexture(string textId);
};

#endif //_CVIEWELEMENTSQUARE_H_
