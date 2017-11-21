#include "CCommonUtils.h"

CCommonUtils* CCommonUtils::m_pInstance = nullptr;

CCommonUtils::CCommonUtils()
{
// no implementation
}

CCommonUtils* CCommonUtils::instance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CCommonUtils();
	}

	return m_pInstance;
}

GLuint 
CCommonUtils::RetrieveOGLDrawDirective(Types::EDrawDirective drawDirective)
{
	// return of this function
	GLuint retVal;
	// switch case evaluation
	switch (drawDirective)
	{
		case Types::DrawDirective_Triangle_Fan:
			retVal = GL_TRIANGLE_FAN;
			break;
		case Types::DrawDirective_Triangle_Strip:
			retVal = GL_TRIANGLE_STRIP;
			break;
		case Types::DrawDirective_Triangles:
		default:
			retVal = GL_TRIANGLES;
			break;
	}

	return retVal;
}

