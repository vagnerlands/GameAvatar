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
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glPushMatrix();
}


void 
CViewElementTerrainMesh::VRender()
{
	if (!loadShader("water2d"))
	{
		return;
	}

	applyTexture("water.bmp");

	TInt32 err = 0;
	glUseProgram(m_pProgramShader->GetProgramObject());
	err = glGetError();
	if (err != 0)
	{
		printf("glError after glUseProgram =%d\n", err);
	}

	// leave the GPU performing the matrices transformations
	m_pProgramShader->setUniform3f("translate", m_position.x, m_position.y, m_position.z);
	m_pProgramShader->setUniform3f("scale", m_scale.x, m_scale.y, m_scale.z);
	m_pProgramShader->setUniform4f("rotation", m_rotate.y, 0.0f, 1.0f, 0.0f);
	static float incTime = 0;
	incTime += 0.001f;
	//static bool signal = true;
	//if (signal)
	//{
	//	incTime += 0.001f;
	//}
	//else
	//{
	//	incTime -= 0.001f;
	//}
	//if (incTime >= 0.5f) signal = false;
	//if (incTime <= 0.0f) signal = true;
	m_pProgramShader->setUniform1f("time_0_X", incTime);
	static float wavefreq = 0.0f;
	wavefreq += 0.0001;
	m_pProgramShader->setUniform1f("wavefreq", wavefreq);
	err = glGetError();
	if (err != 0)
	{
		printf("glError after setUniform1f time =%d\n", err);
	}
	m_pProgramShader->setUniform1f("waveSpeed", 0.18f);

	err = glGetError();
	if (err != 0)
	{
		printf("glError after setUniform1f waveSpeed =%d\n", err);
	}

	//m_pProgramShader->setUniform1f("noiseSpeed", 0.34f);
	m_pProgramShader->setUniform4f("view_position", 0.0f, 30.f, 70.0f, 1.0f);
	//m_pProgramShader->setUniform1f("fadeExp", incTime * 6.08f);
	//m_pProgramShader->setUniform1f("fadeBias", incTime*0.12f);
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
	glEnable(GL_TEXTURE_3D);
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glNormal3f(0.0F, 1.0F, 0.0F);
	glVertex3f(verts[0], verts[1], verts[2]);	
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.0F, 1.0F, 0.0F);
	glVertex3f(verts[3], verts[4], verts[5]);	
	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(0.0F, 1.0F, 0.0F);
	glVertex3f(verts[6], verts[7], verts[8]);
	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(0.0F, 1.0F, 0.0F);
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
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP);

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

	GLint loc = m_pProgramShader->GetUniformLocation("myTextureSampler");
	if (loc != -1)
	{
		TInt32 err = 0;
		err = glGetError();
		if (err != 0)
		{
			//printf("glError EnableMap=%d\n", err);
		}
		glActiveTexture(GL_TEXTURE0);
		err = 0;
		err = glGetError();
		if (err != 0)
		{
			printf("glError glActiveTexture=%d\n", err);
		}
		glBindTexture(GL_TEXTURE_2D, texture);
		err = 0;
		err = glGetError();
		if (err != 0)
		{
			printf("glError glBindTexture=%d\n", err);
		}

		//m_pProgramShader->setUniform1i("myTextureSampler", GL_TEXTURE0);
		//err = 0;
		//err = glGetError();
		//if (err != 0)
		//{
		//	printf("glError setUniform1i=%d\n", err);
		//}
	}

	/*GLint loc = m_pProgramShader->GetUniformLocation("skyBox");
	if (loc != -1)
	{
		TInt32 err = 0;
		err = glGetError();
		glActiveTexture(GL_TEXTURE0);
		if (err != 0)
		{
			printf("glError ActiveTextureMap=%d\n", err);
		}
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		err = glGetError();
		if (err != 0)
		{
			printf("glError TexEnvf=%d\n", err);
		}
		glEnable(GL_TEXTURE_CUBE_MAP);
		err = glGetError();
		if (err != 0)
		{
			printf("glError EnableMap=%d\n", err);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		err = glGetError();
		if (err != 0)
		{
			printf("glError BindMap=%d\n", err);
		}
		//m_pProgramShader->setUniform1i("skyBox", GL_TEXTURE0);
		//err = glGetError();
		//if (err != 0)
		//{
		//	printf("glError CubeMap=%d\n", err);
		//}
	}*/


	/*texture = CTextManager::instance()->getTextureById("3dnoise");
	// requested texture was not found (the requested texture was put in a process queue and in next request it may be there)
	if (texture == -1)
	{
		return;
	}

	loc = m_pProgramShader->GetUniformLocation("Noise");
	if (loc != -1)
	{
		TInt32 err = 0;
		glActiveTexture(GL_TEXTURE1);
		if (err != 0)
		{
			printf("glError ActiveTextureNoise=%d\n", err);
		}
		glBindTexture(GL_TEXTURE_3D, texture);
		if (err != 0)
		{
			printf("glError BindTextureNoise=%d\n", err);
		}
		m_pProgramShader->setUniform1i("Noise", GL_TEXTURE1);
		if (err != 0)
		{
			printf("glError SetUniformNoise=%d\n", err);
		}
	}*/
	
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