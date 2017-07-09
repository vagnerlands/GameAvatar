#ifndef _CVIEWLIGHTDIFFUSE_H_
#define _CVIEWLIGHTDIFFUSE_H_

#include <iostream>
#include "CCommonTypes.h"
#include "IViewLight.h"
#include "GL/glut.h"
using namespace Types;

class CViewLightDiffuse : public IViewLight
{
public:

	CViewLightDiffuse(TFloat posX, TFloat posY, TFloat width, TFloat height);
	// constructor - every object starts as displayed
	CViewLightDiffuse()
	{
		m_isVisible = true;
	}

	virtual ~CViewLightDiffuse()
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

#endif //_CVIEWLIGHTDIFFUSE_H_
