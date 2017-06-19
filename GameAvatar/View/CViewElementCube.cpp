#include "CHumanView.h"
#include "CViewElementCube.h"
#include "CModelManager.h"

CViewElementCube::CViewElementCube(TInt32 posX, TInt32 posY, TInt32 width, TInt32 height)
{
	m_posX = posX;
	m_posY = posY;
	m_width = width;
	m_height = height;
}

CViewElementCube::~CViewElementCube()
{

}

void CViewElementCube::VPreRender()
{
	glPushMatrix();
}


void CViewElementCube::VRender()
{
	loadModel("Wolf.obj");
	glPopMatrix();
}

void CViewElementCube::applyTexture(string textId)
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

	if (err != 0)
	{
		printf("glError GL_BLEND=%d\n", err);
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (err != 0)
	{
		printf("glError glBindTexture=%d\n", err);
	}

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	if (err != 0)
	{
		printf("glError glTexEnvf=%d\n", err);
	}
}

void 
CViewElementCube::loadModel(string modelId)
{
	static float anglerot = 0.0f;
	float variant1 = 0.1f;
	float variant2 = 0.15f;
	anglerot += 0.005f;
	// get texture from cache
	if (!CModelManager::instance()->getModelById(modelId, m_data))
	{
		return;
	}

	//glScalef(0.5f, 0.5f, 0.5f);
	//glScalef(20.0f, 20.0f, 20.0f);
	glScalef(m_width, m_height, 1.0f);
	glTranslatef(m_posX, m_posY, 100.0f);
	glRotatef(anglerot, 0.5f, 0.2f, 0.2f);
	// allocates from cache texture of "Fur"
	applyTexture("furTex.bmp");

	// enable vertices array pointer rendering
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	// points to array of vertices
	glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)&m_data.m_vertices[0]);
	// points to array of textures
	glTexCoordPointer(3, GL_FLOAT, 0, (GLvoid*)&m_data.m_textures[0]);
	// commit the vertices to the OpenGL
	glDrawArrays(GL_TRIANGLES, 0, m_data.m_vertices.size());
	// disable vertices array pointer rendering
	glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glDisableClientState(GL_VERTEX_ARRAY);
	// disable texture 	
	glDisable(GL_TEXTURE_2D);
}