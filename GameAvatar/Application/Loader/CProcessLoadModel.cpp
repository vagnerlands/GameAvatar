#include "CProcessLoadModel.h"
#include "CModelManager.h"

ProcessType 
CProcessLoadModel::VGetType()
{
	return ProcessType_LoadModel;
}

void 
CProcessLoadModel::load()
{
	CModelManager::instance()->LoadModel(m_objectId);
}
