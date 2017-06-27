#include "CHumanView.h"
#include "CViewElementModel.h"
#include "CModelManager.h"
#include "CShaderManager.h"

CViewElementModel::CViewElementModel(TFloat posX, TFloat posY, TFloat width, TFloat height, TFloat volume, string modelName)
{
	m_position.x = posX;
	m_position.y = posY;
	m_position.z = 0.0f;
	m_scale.x = width;
	m_scale.y = height;
	m_scale.z = volume;
	m_modelId = modelName;
}

CViewElementModel::~CViewElementModel()
{

}

void CViewElementModel::VPreRender()
{
	glPushMatrix();
}


void CViewElementModel::VRender()
{
	//if (!loadShader("simple"))
	{
		//return;
	}

	if (!loadModel(m_modelId))
	{
		return;
	}

	glTranslatef(m_position.x, m_position.y, m_position.z);
	glScalef(m_scale.x, m_scale.y, m_scale.z);	
	//glRotatef(m_rotate.x, 1.0f, 0.0f, 0.0f);
	glRotatef(m_rotate.y, 0.0f, 1.0f, 0.0f);
	//glRotatef(m_rotate.z, 0.0f, 0.0f, 1.0f);



	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// enable vertices array pointer rendering
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_CULL_FACE);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	// points to array of vertices
	glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)&m_data.m_vertices[0]);
	if (m_data.m_normals.size() > 0)
	{
		glNormalPointer(GL_FLOAT, 0, &m_data.m_normals[0]);
	}

	// points to array of textures
	if (m_data.m_textures.size() > 0)
	{
		glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)&m_data.m_textures[0]);
	}

	// commit the vertices to the OpenGL
	glDrawArrays(GL_TRIANGLES, 0, m_data.m_vertices.size());
	// disable vertices array pointer rendering
	glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	TInt32 err = glGetError();
	if (err != 0)
	{
		printf("glError Drawing Model =%d\n", err);
	}

	// disable texture 	
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void CViewElementModel::applyTexture(string textId)
{
	// get texture from cache
	GLuint texture = CTextManager::instance()->getTextureById(textId);
	// requested texture was not found (the requested texture was put in a process queue and in next request it may be there)
	if (texture == -1)
	{
		return;
	}

	glEnable(GL_TEXTURE_2D);
	TInt32 err = glGetError();
	if (err != 0)
	{
		printf("glError EnableTexture2D=%d\n", err);
	}
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	err = glGetError();
	if (err != 0)
	{
		printf("glError GL_BLEND=%d\n", err);
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	err = glGetError();
	if (err != 0)
	{
		printf("glError glBindTexture=%d\n", err);
	}

}

bool 
CViewElementModel::loadModel(string modelId)
{
	// get texture from cache
	if (!CModelManager::instance()->getModelById(modelId, m_data))
	{
		return false;
	}

	return true;
}

bool 
CViewElementModel::loadShader(string shaderId)
{
	// get texture from cache
	if (!CShaderManager::instance()->getShaderById(shaderId))
	{
		return false;
	}

	return true;
}