#include "CHumanView.h"
#include "CViewElementModel.h"
#include "CModelManager.h"
#include "CShaderManager.h"
#include "CCommonUtils.h"

CViewElementModel::CViewElementModel(TFloat posX, TFloat posY, TFloat posZ, TFloat width, TFloat height, TFloat volume, string modelName, Types::EDrawDirective drawDirective)
{
	m_position.x = posX;
	m_position.y = posY;
	m_position.z = posZ;
	m_scale.x = width;
	m_scale.y = height;
	m_scale.z = volume;
	m_modelId = modelName;
	m_drawDirective = drawDirective;
}

CViewElementModel::~CViewElementModel()
{

}

void CViewElementModel::VPreRender()
{
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
}


void CViewElementModel::VRender()
{
	if (!loadShader("model"))
	{
		return;
	}

	if (!loadModel(m_modelId))
	{
		return;
	}

	applyTexture("furTex.bmp");

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

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindVertexArray(m_data.m_vertexArrayObject);
	glDrawElements(CCommonUtils::instance()->RetrieveOGLDrawDirective(m_drawDirective), m_data.m_indexes.size(), GL_UNSIGNED_SHORT, (void*)(0));
	err = glGetError();
	if (err != 0)
	{
		printf("glError Drawing Model =%d\n", err);
	}
	
}

void CViewElementModel::VPostRender()
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
	glActiveTexture(GL_TEXTURE0);
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
	if ((m_pProgramShader = CShaderManager::instance()->getShaderById(shaderId)) == 0)
	{
		return false;
	}

	return true;
}