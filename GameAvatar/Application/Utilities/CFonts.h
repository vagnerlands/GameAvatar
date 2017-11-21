#ifndef _CFONTS_H_
#define _CFONTS_H_

#include "CCommonTypes.h"

using namespace Types;

class CFonts
{
public:

	static CFonts* instance();

	Types::SFontAttributes GetFont(string fontName);

private:
	// private constructor - singleton
	CFonts();

	static CFonts* m_pInstance;

	Types::FontMap m_fonts;


};

#endif //_CFONTS_H_