#include "CFonts.h"

CFonts* CFonts::m_pInstance = nullptr;

CFonts::CFonts()
{
	m_fonts.insert(make_pair("mono8", Types::SFontAttributes(8, 15, GLUT_BITMAP_8_BY_13)));
}

CFonts * CFonts::instance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CFonts();
	}

	return m_pInstance;
}

Types::SFontAttributes 
CFonts::GetFont(string fontName)
{
	FontMap::iterator it = m_fonts.find(fontName);
	if (it != m_fonts.end())
	{
		return it->second;
	}
	else
	{
		exit(1);
	}
}
