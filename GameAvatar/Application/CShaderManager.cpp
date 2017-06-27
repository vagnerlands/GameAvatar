#include "CShaderManager.h"
#include "CGameCockpit.h"
#include "CProcessLoadShader.h"
#include "gl/glut.h"
#include <time.h>
#include <iostream>
#include <string>
#ifdef _WIN_
#include "CWinMutex.h"
#endif

CShaderManager* CShaderManager::s_pInstance = NULL;

CShaderManager*
CShaderManager::instance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CShaderManager();
	}

	return s_pInstance;
}

void 
CShaderManager::OnRemoveEvent(string removeItem)
{
	instance()->RemoveShader(removeItem);
}

CShaderManager::CShaderManager()
{
#ifdef _WIN_
	m_pShaderContentMapMutex = new CWinMutex();
#else
#error "no implementation for this platform"
#endif
	if (m_pShaderContentMapMutex != NULL)
	{
		m_pShaderContentMapMutex->createMutex("ShaderContentMap");
	}
	
}

void 
CShaderManager::LoadShader(const string shaderId)
{
	// start loading measuring time
	clock_t start = clock();

	// read data from file
	cwc::glShader* obj = 0;
	char vertexFilename[200], fragmentFilename[200];
	sprintf(vertexFilename, "./%sVertexshader.txt", shaderId.data());
	sprintf(fragmentFilename, "./%sFragmentshader.txt", shaderId.data());

	obj = m_shaderManager.loadfromFile(vertexFilename, fragmentFilename);

	if (obj != 0)
	{
		m_shaders.insert(make_pair(shaderId, obj));
	}
	else
	{
		printf("<!> Failed to parse shader files [%s]\n", shaderId);
	}

	// time measurement
	printf(" loading shader [%s] %.2fms\n", shaderId.data(), (float)(clock() - start));
}

void 
CShaderManager::RemoveShader(const string shaderId)
{
	ShadersMap::iterator it = m_shaders.find(shaderId);
	if (it == m_shaders.end())
	{
		// this shouldn't happen - never, but if happens, trying 
		// to erase will cause an exception - so must quit method
		return;
	}

	m_shaders.erase(it);

	//_CrtDumpMemoryLeaks();
}

cwc::glShader*
CShaderManager::getShaderById(string shaderId)
{
	// then try to find it in the textures map
	ShadersMap::iterator result = m_shaders.find(shaderId);
	if (result != m_shaders.end())
	{
		return result->second;
	}

	// cache miss - then add this texture to the process list
	shared_ptr<CProcessLoadShader> pProcess(new CProcessLoadShader(shaderId));
	CGameCockpit::instance()->pushProcess(pProcess);
	
	// if it somehow failed, returns -1
	return false;
}


CShaderManager::~CShaderManager()
{
	for (auto it = m_shaders.begin(); it != m_shaders.end();) {
		m_shaders.erase(it);
	}		
}