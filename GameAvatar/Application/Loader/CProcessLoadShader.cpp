#include "CProcessLoadShader.h"
#include "CShaderManager.h"

ProcessType 
CProcessLoadShader::VGetType()
{
	return ProcessType_LoadShader;
}

void 
CProcessLoadShader::load()
{
	CShaderManager::instance()->LoadShader(m_objectId);
}
