#ifndef CSHADERMANAGER_H_
#define CSHADERMANAGER_H_

#include "CCommonTypes.h"
#include "IMutex.h"
#include <unordered_map>
#include <memory>

using namespace Types;
using namespace std;

class CShaderManager
{
public:
	static CShaderManager* instance();
	void LoadShader(const string modelId);
	void RemoveShader(const string modelId);
	cwc::glShader* getShaderById(string shaderId);
	CShaderManager::~CShaderManager();
	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(string removeItem);


private:
	CShaderManager();
	// local hashmap built textures
	ShadersMap m_shaders;
	// mutex for m_processes
	IMutex* m_pShaderContentMapMutex;
	// local instance
	static CShaderManager* s_pInstance;
	cwc::glShaderManager m_shaderManager;
};

#endif //CSHADERMANAGER_H_