#ifndef _IVIEWELEMENT_H_
#define _IVIEWELEMENT_H_

#include <iostream>
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
	virtual ~IViewElement()
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

	virtual void VPostRender() = 0;
	// debug purposes
	virtual char* VGetName()
	{
		return "NoNameElement";
	}

	virtual GameViewElement VGetType()
	{
		return GameViewElement_Undefined;
	}

	virtual void VScale(TFloat x, TFloat y, TFloat z)
	{
		m_scale.x *= x;
		m_scale.y *= y;
		m_scale.z *= z;
	}

	virtual void VRotateX(TFloat x)
	{
		m_rotate.x += x;
		if (m_rotate.x > 360.0f) m_rotate.x -= 360.0f;
		else if (m_rotate.x < 0.0f) m_rotate.x += 360.0f;
	}

	virtual void VRotateY(TFloat y)
	{
		m_rotate.y += y;
		if (m_rotate.y > 360.0f) m_rotate.y = 0.0f;
		else if (m_rotate.y < 0.0f) m_rotate.y = 360.0f;
		std::cout << m_rotate.y << std::endl;
	}

	virtual void VRotateZ(TFloat z)
	{
		m_rotate.z += z;
		if (m_rotate.z > 360.0f) m_rotate.z -= 360.0f;
		else if (m_rotate.z < 0.0f) m_rotate.z += 360.0f;
	}

	virtual void VTranslateX(TFloat x)
	{
		m_position.x += x;
	}

	virtual void VTranslateY(TFloat y)
	{
		m_position.y += y;
	}

	virtual void VTranslateZ(TFloat z)
	{
		m_position.z += z;
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

	bool m_isVisible;
	glm::vec3 m_position;
	glm::vec3 m_rotate;
	glm::vec3 m_scale;
};

#endif
