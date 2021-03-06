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

CModelManager::~CModelManager()
{
	while (!m_models.empty())
	{
		m_models.erase(m_models.begin());
	}	
}

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
	CResource resModel(modelId);
	shared_ptr<CResHandle> modelStream = m_cacheDb.GetHandle(&resModel);
	string materialId = modelId.substr(0, modelId.find_last_of('.'));
	materialId += ".mtl";
	CResource resMaterial(materialId);
	shared_ptr<CResHandle> materialStream = m_cacheDb.GetHandle(&resMaterial);
	if (modelStream->GetSize() > 0)
	{
		Byte* data = modelStream->Buffer();
		Byte* materialData = 0;
		if (materialStream->GetSize() > 0) materialData = materialStream->Buffer();
		AddModelContent(modelId, data, materialData);
	}
	else
	{
		printf("Model %s not found in resource file!\n", modelId);
	}	

	// time measurement
	printf(" loading model [%s] %.2fms\n", modelId.data(), (float)(clock() - start));
}

void
CModelManager::AddModelContent(string modelId, Byte* bytesStream, Byte* materialStream)
{
	// new node to be allocated
	SModelData newNode;
	// will be deallocated at the end of this function
	shared_ptr<objLoader> objData (new objLoader());
	// parses the loaded model
	objData->load(bytesStream, materialStream, true);

	printf("Number of vertices: %i\n", objData->vertexCount);
	printf("Number of vertex normals: %i\n", objData->normalCount);
	printf("Number of texture coordinates: %i\n", objData->textureCount);
	printf("\n");

	newNode.m_vboBufferCreated = false;

	for (int i = 0; i < objData->materialCount; i++)
	{
		SMaterialAttr newMaterial;
		obj_material* m = objData->materialList[i];

		newMaterial.m_ambient = glm::vec3(m->amb[0], m->amb[1], m->amb[2]);
		newMaterial.m_specular = glm::vec3(m->spec[0], m->spec[1], m->spec[2]);
		newMaterial.m_diffuse = glm::vec3(m->diff[0], m->diff[1], m->diff[2]);
		newMaterial.m_glossy = m->glossy;
		newMaterial.m_reflect = m->reflect;
		newMaterial.m_refract = m->refract;
		newMaterial.m_shiny = m->shiny;
		newMaterial.m_trans = m->trans;

		newNode.m_material.push_back(newMaterial);
	}

	bool recalculateNormals = true;

	if (objData->normalCount > 0)
	{
		recalculateNormals = false;
		for (int i = 0; i < objData->normalCount; i++)
		{
			// temporary normal object
			obj_vector* n = objData->normalList[i];
			newNode.m_normals.push_back(glm::vec3(n->e[0], n->e[1], n->e[2]));
		}
	}

	for (int i = 0; i < objData->vertexCount; i++)
	{
		// temporary vertex object
		obj_vector *o = objData->vertexList[i];

		newNode.m_vertices.push_back(glm::vec3(o->e[0], o->e[1], o->e[2]));

		if (recalculateNormals)
		{
			//! if normals are not present - calculate them!
			// calculate the length of the axis of the vertex
			Float length = sqrt(pow(o->e[0], 2.0)
				+ pow(o->e[1], 2.0)
				+ pow(o->e[2], 2.0));

			// use calculate normals 
			// divide the vertex by the calculate length of the vertexes
			newNode.m_normals.push_back(glm::vec3(o->e[0] / length, o->e[1] / length, o->e[2] / length));
		}
	}

	for (int i = 0; i < objData->textureCount; i++)
	{
		obj_vector* t = objData->textureList[i];
		newNode.m_textures.push_back(glm::vec2(t->e[0], t->e[1]));
	}

	for (int i = 0; i < objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];

		for (int j = 0; j < o->vertex_count; j++)
		{
			
			//newNode.m_normalsIndexed.push_back(o->normal_index[j]);
			newNode.m_indexes.push_back(o->vertex_index[j]);
			//if (o->material_index != -1)
			//{
			//	newNode.m_texturesIndexed.push_back(o->texture_index[j]);
			//}			
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
		
		if (!result->second.m_vboBufferCreated)
		{			
			result->second.m_vboBufferCreated = true;
			
			// generates the VAO
			glGenVertexArrays(1, &result->second.m_vertexArrayObject);
			// binds the VAO for this model
			glBindVertexArray(result->second.m_vertexArrayObject);
			// [          Load buffers          ]
			//vertices
			glGenBuffers(Types::VertexBuffer_Max_Num, result->second.m_elementBuffer);
			int oglErr = glGetError();
			if (oglErr != 0)
			{
				printf("glError after glGenBuffers =%d\n", glGenBuffers);
			}

			// [VERTEX PREPARATION]
			glBindBuffer(GL_ARRAY_BUFFER, result->second.m_elementBuffer[Types::VertexBuffer_Vertices]);
			glBufferData(GL_ARRAY_BUFFER, (result->second.m_vertices.size() * sizeof(glm::vec3)), &result->second.m_vertices[0], GL_STATIC_DRAW);
			glVertexPointer(3, GL_FLOAT, 0, (void*)(0));
			// [------------------]

			// [NORMAL PREPARATION]
			glBindBuffer(GL_ARRAY_BUFFER, result->second.m_elementBuffer[Types::VertexBuffer_Normals]);
			glBufferData(GL_ARRAY_BUFFER, (result->second.m_normals.size() * sizeof(glm::vec3)), &result->second.m_normals[0], GL_STATIC_DRAW);
			glNormalPointer(GL_FLOAT, 0, (void*)(0));
			// [------------------]

			// [TEXTURE PREPARATION]
			if (result->second.m_textures.size() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, result->second.m_elementBuffer[Types::VertexBuffer_Textures]);
				glBufferData(GL_ARRAY_BUFFER, (result->second.m_textures.size() * sizeof(glm::vec2)), &result->second.m_textures[0], GL_STATIC_DRAW);
				glTexCoordPointer(2, GL_FLOAT, 0, (void*)(0));
			}
			// [------------------]

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->second.m_elementBuffer[Types::VertexBuffer_Element]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * result->second.m_indexes.size(), &result->second.m_indexes[0], GL_STATIC_DRAW);

			// allocation integrity check
			Int32 allocIntegrityChk = 0;
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &allocIntegrityChk);
			if (allocIntegrityChk != result->second.m_indexes.size() * sizeof(GLushort))
			{
				glDeleteBuffers(Types::VertexBuffer_Max_Num, result->second.m_elementBuffer);
				printf(" <!> error while allocating element buffer in %s\n", textId.data());
			}
			// [--------------------------------]
		}

		out = result->second;

		return true;
	}

	// cache miss - then add this texture to the process list
	shared_ptr<CProcessLoadModel> pProcess(new CProcessLoadModel(textId));
	CGameCockpit::instance()->pushProcess(pProcess);
	
	// if it somehow failed, returns -1
	return false;
}
