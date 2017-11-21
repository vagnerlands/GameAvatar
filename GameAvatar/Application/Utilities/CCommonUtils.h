#ifndef _CCOMMONUTILS_H_
#define _CCOMMONUTILS_H_

#include "CCommonTypes.h"

using namespace Types;

class CCommonUtils
{
public:

	static CCommonUtils* instance();

	GLuint RetrieveOGLDrawDirective(Types::EDrawDirective drawDirective);

private:
	// private constructor - singleton
	CCommonUtils();

	static CCommonUtils* m_pInstance;

};

#endif //_CFONTS_H_