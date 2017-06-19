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
		printf(" face ");
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

			if (objData->textureCount > 0)
			{
				newNode.m_textures.push_back(glm::vec2(
					objData->textureList[o->vertex_index[j]]->e[0],
					objData->textureList[o->vertex_index[j]]->e[1]
				));
			}
		}
		//printf("\n");
	}
	/*
	for (int i = 0; i < objData->normalCount; i++)
	{
		obj_vector* o = objData->normalList[i];
		printf(" normals ");
		for (int j = 0; j<3; j++)
		{			
			newNode.m_normals.push_back(glm::vec3(
				objData->normalList[o->vertex_index[j]]->e[0],
				objData->normalList[o->vertex_index[j]]->e[1],
				objData->normalList[o->vertex_index[j]]->e[2]
			));
		}
	}

	for (int i = 0; i < objData->textureCount; i++)
	{
		obj_vector* o = objData->textureList[i];
		printf(" texture ");
		for (int j = 0; j<3; j++)
		{
			newNode.m_textures.push_back(glm::vec2(
				o->e[0],
				o->e[1]
			));
		}
	}
	*/
	m_pModelContentMapMutex->mutexLock();
	m_models.insert(make_pair(modelId, newNode));
	m_pModelContentMapMutex->mutexUnlock();

	// objData will lose its last reference and it will be deallocated
}

/*
void 
CModelManager::AddModelContent(string modelId, TByte* bytesStream)
{
	SModelData newNode;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	membuf sbuf(bytesStream, bytesStream + strlen(bytesStream));
	std::istream streamIn(&sbuf);

	while (1) 
	{
		string lineHeader;
		// read the first word of the line

		if (!std::getline(streamIn, lineHeader))
			break; // EOF = End Of File. Quit the loop.

		if (memcmp(lineHeader.c_str(), "v", 1) == 0) {
			glm::vec3 vertex;
			sscanf(lineHeader.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (memcmp(lineHeader.c_str(), "vt", 2) == 0) {
			glm::vec2 uv;
			sscanf(lineHeader.c_str(), "vt %f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (memcmp(lineHeader.c_str(), "vn", 2) == 0) {
			glm::vec3 normal;
			sscanf(lineHeader.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (memcmp(lineHeader.c_str(), "f", 1) == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			//int matches = sscanf("5/1/1 1/2/1 4/3/1", "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			int matches = sscanf(lineHeader.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return;
			}
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

//			newNode.m_vertices.push_back(glm::vec3(vertexIndex[0], vertexIndex[1], vertexIndex[2]));
			//newNode.m_normals.push_back(glm::vec3(normalIndex[0], normalIndex[1], normalIndex[2]));
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		newNode.m_vertices.push_back(vertex);
	}

	m_pModelContentMapMutex->mutexLock();
	m_models.insert(make_pair(modelId, newNode));
	m_pModelContentMapMutex->mutexUnlock();
}
*/
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
/*
void 
CModelManager::AddModelContent(string modelId, shared_ptr<CResHandle> data)
{
	SModelData newNode;
	m_pModelContentMapMutex->mutexLock();
	m_models.insert(make_pair(textId, newNode));
	m_pModelContentMapMutex->mutexUnlock();
}
*/
CModelManager::~CModelManager()
{
	for (auto it = m_models.begin(); it != m_models.end();) {
		m_models.erase(it);
	}		
}