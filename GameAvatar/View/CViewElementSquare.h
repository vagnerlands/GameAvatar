#ifndef _CVIEWELEMENTSQUARE_H_
#define _CVIEWELEMENTSQUARE_H_

#include "IViewElement.h"

class CViewElementSquare : public IViewElement
{
public:

	CViewElementSquare(Float posX, Float posY, Float width, Float height, string textureName);

	virtual ~CViewElementSquare();

	virtual void VPreRender();

	virtual void VRender();

	virtual void VPostRender();

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

	bool loadShader(string shaderId);

	string m_textureId;

};

#endif //_CVIEWELEMENTSQUARE_H_
