#ifndef _IVIEWLIGHT_H_
#define _IVIEWLIGHT_H_

#include <iostream>
#include "CCommonTypes.h"
#include "IViewElement.h"
#include "GL/glut.h"
using namespace Types;

class IViewLight : public IViewElement
{
public:
	// constructor - every object starts as displayed
	IViewLight()
	{
		m_isVisible = true;
	}

	virtual ~IViewLight()
	{

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
		return "Light_Interface";
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

	// bind texture 
	virtual bool loadModel(string textId)
	{
		return true;
	}
};

#endif //_IVIEWLIGHT_H_
