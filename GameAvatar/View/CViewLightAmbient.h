#ifndef _CVIEWLIGHTAMBIENT_H_
#define _CVIEWLIGHTAMBIENT_H_

#include <iostream>
#include "CCommonTypes.h"
#include "IViewLight.h"
#include "GL/glut.h"
using namespace Types;

class CViewLightAmbient : public IViewLight
{
public:
	// constructor - every object starts as displayed

	CViewLightAmbient(TFloat posX, TFloat posY, TFloat width, TFloat height);

	CViewLightAmbient()
	{
		m_isVisible = true;
	}

	virtual ~CViewLightAmbient()
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
	// debug purposes
	virtual char* VGetName()
	{
		return "Light_Ambient";
	}

	virtual GameViewElement VGetType()
	{
		return GameViewElement_Light_Ambient;
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
