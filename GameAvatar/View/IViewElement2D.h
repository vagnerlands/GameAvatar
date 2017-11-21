#ifndef _IVIEWELEMENT2D_H_
#define _IVIEWELEMENT2D_H_

#include <iostream>
#include "IViewElement.h"
#include "CCommonTypes.h"
#include "CTextManager.h"
#include "GL/glut.h"
using namespace Types;

class IViewElement2D : public IViewElement
{
public:
	// constructor - every object starts as displayed
	IViewElement2D()
	{
		m_isVisible = true;
		m_transparency = 0.0F;
	}
	virtual ~IViewElement2D()
	{

	}

	// prepare matrices, enable/disable directives and so on 
	// before starting to draw the element
	virtual void VPreRender() = 0;
	// actually draws the element
	virtual void VRender() = 0;

	virtual void VPostRender() = 0;
	// debug purposes
	virtual char* VGetName()
	{
		return "NoNameElement2D";
	}

	virtual GameViewElement VGetType()
	{
		return GameViewElement_Undefined;
	}

	virtual void VSetTransparency(TFloat transparency)
	{
		m_transparency = transparency;
	}


protected:
	// bind texture 
	virtual void applyTexture(string textId)
	{
		// empty implementation
	}

	// bind texture 
	virtual bool loadModel(string textId)
	{
		return true;
	}

	virtual bool loadShader(string shaderId)
	{
		return true;
	}

	TFloat m_transparency;

};

#endif //_IVIEWELEMENT2D_H_
