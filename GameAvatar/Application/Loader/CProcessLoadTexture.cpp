#include "CProcessLoadTexture.h"
#include "CTextManager.h"

ProcessType 
CProcessLoadTexture::VGetType()
{
	return ProcessType_LoadTexture;
}

void 
CProcessLoadTexture::load()
{
	CTextManager::instance()->LoadTexture(m_objectId);
}
