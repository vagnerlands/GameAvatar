#include "CHumanView.h"
#include "CViewElementTerrainMesh.h"
#include "CModelManager.h"
#include "CShaderManager.h"
#include "CTerrainDatabaseLoader.h"
#include "CLandscape.h"
#include "CGameCockpit.h"

CViewElementTerrainMesh::CViewElementTerrainMesh(Float posX, Float posY, Float posZ, Float width, Float height, Float volume)
	: m_pLandscape(nullptr)

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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void 
CViewElementTerrainMesh::VRender()
{
	if (!loadShader("model"))
	{
		return;
	}

	applyTexture("groundTex.bmp");
	//glDisable(GL_TEXTURE_2D);

	Int32 err = 0;
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
	static float realAngle = 0.f;
	//realAngle += 0.1f;
	float ang = (3.14 / 180.f) * realAngle;
	// debug - local rotation matrix
	float rotationMatrix[9] = {
		cos(ang), 0.f,  -sin(ang),
		0.f , 1.f , 0.f,
		sin(ang), 0.f, cos(ang)
	};
	// ---

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
	//m_pProgramShader->setUniform1f("time_0_X", incTime);
	static float incrementor = 1.20;
	static float wavefreq = 0.0f;
	wavefreq *= incrementor;
	if (wavefreq > 100.f)
	{
		incrementor = incrementor * -1;
	}
	//m_pProgramShader->setUniform1f("wavefreq", wavefreq);
	err = glGetError();
	if (err != 0)
	{
		printf("glError after setUniform1f time =%d\n", err);
	}
	//m_pProgramShader->setUniform1f("waveSpeed", 0.18f);

	err = glGetError();
	if (err != 0)
	{
		printf("glError after setUniform1f waveSpeed =%d\n", err);
	}

	//m_pProgramShader->setUniform1f("noiseSpeed", 0.34f);
	//m_pProgramShader->setUniform4f("view_position", 0.0f, 30.f, 70.0f, 1.0f);
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

	const UInt32 displayAreaSize = m_pLandscape->GetDisplaySize();
	glBegin(GL_TRIANGLE_STRIP);
	for (Int32 i = 0; i < displayAreaSize; ++i)
	{
		for (Int32 j = 0; j < displayAreaSize; ++j)
		{
			const glm::vec3* meshVertexes = m_pLandscape->GetTerrainMeshVertexes(i, j);
			const glm::vec3* meshNormals = m_pLandscape->GetTerrainMeshNormals(i, j);
			const glm::vec2* meshTextCoords = m_pLandscape->GetTerrainMeshTextureCoordinates(i, j);
			
			glNormal3f(meshNormals[0].x, meshNormals[0].y, meshNormals[0].z);
			glTexCoord2f(meshTextCoords[0].x, meshTextCoords[0].y);
			glVertex3f(meshVertexes[0].x, meshVertexes[0].y, meshVertexes[0].z);							
			glTexCoord2f(meshTextCoords[1].x, meshTextCoords[1].y);
			//glNormal3f(meshNormals[1].x, meshNormals[1].y, meshNormals[1].z);
			glVertex3f(meshVertexes[1].x, meshVertexes[1].y, meshVertexes[1].z);
			
			glTexCoord2f(meshTextCoords[2].x, meshTextCoords[2].y);
			//glNormal3f(meshNormals[2].x, meshNormals[2].y, meshNormals[2].z);
			glVertex3f(meshVertexes[2].x, meshVertexes[2].y, meshVertexes[2].z);

			//glTexCoord2f(meshTextCoords[3].x, meshTextCoords[3].y);
			//glNormal3f(meshNormals[3].x, meshNormals[3].y, meshNormals[3].z);
			glVertex3f(meshVertexes[3].x, meshVertexes[3].y, meshVertexes[3].z);
			glTexCoord2f(meshTextCoords[3].x, meshTextCoords[3].y);
			//glNormal3f(meshNormals[1].x, meshNormals[1].y, meshNormals[1].z);
			//glVertex3f(meshVertexes[1].x, meshVertexes[1].y, meshVertexes[1].z);				
			//glTexCoord2f(meshTextCoords[2].x, meshTextCoords[2].y);
			//glNormal3f(meshNormals[2].x, meshNormals[2].y, meshNormals[2].z);
			//glVertex3f(meshVertexes[2].x, meshVertexes[2].y, meshVertexes[2].z);
			
		}
	}
	glEnd();

	/*glPushMatrix();
	glTranslatef(0, 1.f, 0);
	glUseProgram(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(0, 1, 0);
	for (Int32 i = 0; i < displayAreaSize; ++i)
	{
		for (Int32 j = 0; j < displayAreaSize; ++j)
		{
			const glm::vec3* meshVertexes = m_pLandscape->GetTerrainMeshVertexes(i, j);
			const glm::vec3* meshNormals = m_pLandscape->GetTerrainMeshNormals(i, j);
			const glm::vec2* meshTextCoords = m_pLandscape->GetTerrainMeshTextureCoordinates(i, j);
			glBegin(GL_TRIANGLE_STRIP);
			//glNormal3f(meshNormals[0].x, meshNormals[0].y, meshNormals[0].z);
			//glNormal3f(0, 1, 0);;
			glTexCoord2f(meshTextCoords[0].x, meshTextCoords[0].y);
			glVertex3f(meshVertexes[0].x, meshVertexes[0].y, meshVertexes[0].z);
			glTexCoord2f(meshTextCoords[1].x, meshTextCoords[1].y);
			glVertex3f(meshVertexes[1].x, meshVertexes[1].y, meshVertexes[1].z);
			glTexCoord2f(meshTextCoords[2].x, meshTextCoords[2].y);
			glVertex3f(meshVertexes[2].x, meshVertexes[2].y, meshVertexes[2].z);

			//glNormal3f(meshNormals[0].x, meshNormals[0].y, meshNormals[0].z);
			//glNormal3f(meshNormals[1].x, meshNormals[1].y, meshNormals[1].z);
			//glNormal3f(0, 1, 0);
			glTexCoord2f(meshTextCoords[3].x, meshTextCoords[3].y);
			glVertex3f(meshVertexes[3].x, meshVertexes[3].y, meshVertexes[3].z);
			glTexCoord2f(meshTextCoords[1].x, meshTextCoords[1].y);
			glVertex3f(meshVertexes[1].x, meshVertexes[1].y, meshVertexes[1].z);
			glTexCoord2f(meshTextCoords[2].x, meshTextCoords[2].y);
			glVertex3f(meshVertexes[2].x, meshVertexes[2].y, meshVertexes[2].z);
			glEnd();
		}
	}
	glPopMatrix();*/
	//glBindVertexArray(m_data.m_vertexArrayObject);
	//glDrawElements(GL_TRIANGLES, m_data.m_indexes.size(), GL_UNSIGNED_SHORT, (void*)(0));
	err = glGetError();
	if (err != 0)
	{
		printf("glError Drawing Model =%d\n", err);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void CViewElementTerrainMesh::VPostRender()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	Int32 err = glGetError();
	if (err != 0)
	{
		printf("glError Disabling states Model = %d\n", err);
	}

	// disable texture 	
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_CUBE_MAP);

	glUseProgram(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
		Int32 err = 0;
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
		Int32 err = 0;
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
		Int32 err = 0;
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