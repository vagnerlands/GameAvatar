#include "CModelManager.h"
#include "CGameCockpit.h"
#include "CProcessLoadModel.h"
#include "gl/glut.h"
#include <time.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <objParser/objLoader.h>
#ifdef _WIN_
#include "CWinMutex.h"
#endif

CModelManager* CModelManager::s_pInstance = NULL;

CModelManager*
CModelManager::instance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CModelManager();
		s_pInstance->m_cacheDb.Init();
	}

	return s_pInstance;
}

void 
CModelManager::OnRemoveEvent(string removeItem)
{
	instance()->RemoveModel(removeItem);
}

CModelManager::CModelManager()
	: m_modelFiles("C:\\Users\\Vagner\\Documents\\Visual Studio 2015\\Projects\\GameAvatar\\GameAvatar\\Resources\\model.zip", this->OnRemoveEvent),
	  m_cacheDb(10, &m_modelFiles)
{
#ifdef _WIN_
	m_pModelContentMapMutex = new CWinMutex();
#else
#error "no implementation for this platform"
#endif
	if (m_pModelContentMapMutex != NULL)
	{
		m_pModelContentMapMutex->createMutex("ModelContentMap");
	}
}

void 
CModelManager::LoadModel(const string modelId)
{
	// start loading measuring time
	clock_t start = clock();

	// cache missed - must reload it from resources db
	CResource resExample(modelId);
	shared_ptr<CResHandle> bytesStream = m_cacheDb.GetHandle(&resExample);
	TByte* data = bytesStream->Buffer();

	AddModelContent(modelId, bytesStream->Buffer());

	// time measurement
	printf(" loading model [%s] %.2fms\n", modelId.data(), (float)(clock() - start));
}

void
CModelManager::AddModelContent(string modelId, TByte* bytesStream)
{
	// new node to be allocated
	SModelData newNode;
	// will be deallocated at the end of this function
	shared_ptr<objLoader> objData (new objLoader());
	// parses the loaded model
	objData->load(bytesStream, true);

	printf("Number of vertices: %i\n", objData->vertexCount);
	printf("Number of vertex normals: %i\n", objData->normalCount);
	printf("Number of texture coordinates: %i\n", objData->textureCount);
	printf("\n");
	for (int i = 0; i < objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];
		//printf(" face ");
		for (int j = 0; j<3; j++)
		{
			newNode.m_vertices.push_back(glm::vec3(
				objData->vertexList[o->vertex_index[j]]->e[0],
				objData->vertexList[o->vertex_index[j]]->e[1],
				objData->vertexList[o->vertex_index[j]]->e[2]
			    ));			

			if (objData->normalCount > 0)
			{
				newNode.m_normals.push_back(glm::vec3(
					objData->normalList[o->vertex_index[j]]->e[0],
					objData->normalList[o->vertex_index[j]]->e[1],
					objData->normalList[o->vertex_index[j]]->e[2]
				));
			}
			else
			{
				//! if normals are not present - calculate them!
				// calculate the length of the axis of the vertex
				TFloat length = sqrt(pow(objData->vertexList[o->vertex_index[j]]->e[0], 2.0)
					+ pow(objData->vertexList[o->vertex_index[j]]->e[1], 2.0)
					+ pow(objData->vertexList[o->vertex_index[j]]->e[2], 2.0));

				// use calculate normals 
				// divide the vertex by the calculate length of the vertexes
				newNode.m_normals.push_back(glm::vec3(
					objData->vertexList[o->vertex_index[j]]->e[0] / length,
					objData->vertexList[o->vertex_index[j]]->e[1] / length,
					objData->vertexList[o->vertex_index[j]]->e[2] / length
				));
			}

			if (objData->textureCount > 0)
			{
				newNode.m_textures.push_back(glm::vec2(
					objData->textureList[o->vertex_index[j]]->e[0],
					objData->textureList[o->vertex_index[j]]->e[1]
				));
			}
			else
			{
				// must think about a way to map textures in case the 
				// mapping isn't present
			}
		}
	}

	m_pModelContentMapMutex->mutexLock();
	m_models.insert(make_pair(modelId, newNode));
	m_pModelContentMapMutex->mutexUnlock();

	// objData will lose its last reference and it will be deallocated
}

void 
CModelManager::RemoveModel(const string textId)
{
	ModelMap::iterator it = m_models.find(textId);
	if (it == m_models.end())
	{
		// this shouldn't happen - never, but if happens, trying 
		// to erase will cause an exception - so must quit method
		return;
	}

	m_models.erase(it);

	//_CrtDumpMemoryLeaks();
}

bool 
CModelManager::getModelById(string textId, SModelData& out)
{
	// then try to find it in the textures map
	ModelMap::iterator result = m_models.find(textId);
	if (result != m_models.end())
	{
		out = result->second;
		return true;
	}

	// cache miss - then add this texture to the process list
	shared_ptr<CProcessLoadModel> pProcess(new CProcessLoadModel(textId));
	CGameCockpit::instance()->pushProcess(pProcess);
	
	// if it somehow failed, returns -1
	return false;
}


CModelManager::~CModelManager()
{
	for (auto it = m_models.begin(); it != m_models.end();) {
		m_models.erase(it);
	}		
}