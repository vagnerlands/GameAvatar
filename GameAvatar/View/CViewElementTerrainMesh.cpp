#include "CHumanView.h"
#include "CViewElementTerrainMesh.h"
#include "CModelManager.h"
#include "CShaderManager.h"

CViewElementTerrainMesh::CViewElementTerrainMesh(TFloat posX, TFloat posY, TFloat posZ, TFloat width, TFloat height, TFloat volume)
{
	m_position.x = posX;
	m_position.y = posY;
	m_position.z = posZ;
	m_scale.x = width;
	m_scale.y = height;
	m_scale.z = volume;
}

CViewElementTerrainMesh::~CViewElementTerrainMesh()
{

}

void 
CViewElementTerrainMesh::VPreRender()
{
	glPushMatrix();
}


void 
CViewElementTerrainMesh::VRender()
{
	if (!loadShader("simpletexture"))
	{
		return;
	}

	applyTexture("mountain.bmp");

	glUseProgram(m_pProgramShader->GetProgramObject());
	TInt32 err = glGetError();
	if (err != 0)
	{
		printf("glError after glUseProgram =%d\n", err);
	}

	// leave the GPU performing the matrices transformations
	// code in the shader
	//static float s_time = 0.0f;
	//s_time += 0.001;
	m_pProgramShader->setUniform3f("translate", m_position.x, m_position.y, m_position.z);
	m_pProgramShader->setUniform3f("scale", m_scale.x, m_scale.y, m_scale.z);
	m_pProgramShader->setUniform4f("rotation", m_rotate.y, 0.0f, 1.0f, 0.0f);
	//m_pProgramShader->setUniform1f("time", s_time);

	err = glGetError();
	if (err != 0)
	{
		printf("glError after setUniform3f =%d\n", err);
	}

	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// enable vertices array pointer rendering
	float verts[] = {
		// square coordinates
		-50.0f,   0.0f,  50.0f,
		-50.0f,   0.0f, -50.0f,
		 50.0f,   0.0f, -50.0f,		
		 50.0f,   0.0f,  50.0f
	};

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(verts[0], verts[1], verts[2]);	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(verts[3], verts[4], verts[5]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(verts[6], verts[7], verts[8]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(verts[9], verts[10], verts[11]);
	glEnd();

	//glBindVertexArray(m_data.m_vertexArrayObject);
	//glDrawElements(GL_TRIANGLES, m_data.m_indexes.size(), GL_UNSIGNED_SHORT, (void*)(0));
	err = glGetError();
	if (err != 0)
	{
		printf("glError Drawing Model =%d\n", err);
	}

}

void CViewElementTerrainMesh::VPostRender()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	TInt32 err = glGetError();
	if (err != 0)
	{
		printf("glError Disabling states Model = %d\n", err);
	}

	// disable texture 	
	glDisable(GL_TEXTURE_2D);

	glUseProgram(0);

	glPopMatrix();
}

void 
CViewElementTerrainMesh::applyTexture(string textId)
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
CViewElementTerrainMesh::loadShader(string shaderId)
{
	// get texture from cache
	if ((m_pProgramShader = CShaderManager::instance()->getShaderById(shaderId)) == 0)
	{
		return false;
	}

	return true;
}