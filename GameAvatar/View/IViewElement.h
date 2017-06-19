#ifndef _IVIEWELEMENT_H_
#define _IVIEWELEMENT_H_

#include "CCommonTypes.h"
#include "CTextManager.h"
#include "GL/glut.h"
using namespace Types;

class IViewElement
{
public:
	// constructor - every object starts as displayed
	IViewElement() 
	{
		m_isVisible = true;
	}
	// set the visibility status
	virtual void VSetVisible(bool visibility)
	{
		m_isVisible = visibility;
	}
	// checks if object shall be displayed or not
	virtual bool VIsVisible()
	{
		return m_isVisible;
	}
	// prepare matrices, enable/disable directives and so on 
	// before starting to draw the element
	virtual void VPreRender() = 0;
	// actually draws the element
	virtual void VRender() = 0;
	// debug purposes
	virtual char* VGetName()
	{
		return "NoNameElement";
	}

	virtual GameViewElement VGetType()
	{
		return GameViewElement_Undefined;
	}

protected:
	// bind texture 
	virtual void applyTexture(string textId)
	{
		// empty implementation
	}
	bool m_isVisible;
	TFloat m_posX, m_posY;
	TFloat m_width, m_height, m_volume;
};

#endif
