#ifndef _CVIEWLIGHTSPECULAR_H_
#define _CVIEWLIGHTSPECULAR_H_

#include <iostream>
#include "CCommonTypes.h"
#include "IViewLight.h"
#include "GL/glut.h"
using namespace Types;

class CViewLightSpecular : public IViewLight
{
public:

	CViewLightSpecular(Float posX, Float posY, Float width, Float height);
	// constructor - every object starts as displayed
	CViewLightSpecular()
	{
		m_isVisible = true;
	}

	virtual ~CViewLightSpecular()
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
	virtual void VPreRender();
	// actually draws the element
	virtual void VRender();

	virtual void VPostRender();
	// debug purposes
	virtual char* VGetName()
	{
		return "Light_Specular";
	}

	virtual GameViewElement VGetType()
	{
		return GameViewElement_Light_Specular;
	}

protected:
	// bind texture 
	virtual void applyTexture(string textId);

	// bind texture 
	virtual bool loadModel(string textId)
	{
		return true;
	}

};

#endif //_IVIEWLIGHT_H_
