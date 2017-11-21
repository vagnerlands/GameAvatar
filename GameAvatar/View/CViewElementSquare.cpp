#include "CHumanView.h"
#include "CViewElementSquare.h"
#include "CShaderManager.h"

CViewElementSquare::CViewElementSquare(TFloat posX, TFloat posY, TFloat width, TFloat height, string textureName)
{
	m_position.x = posX;
	m_position.y = posY;
	m_position.z = 0.0f;
	m_scale.x = width;
	m_scale.y = height;
	m_scale.z = 1.0f;
	m_textureId = textureName;
}

CViewElementSquare::~CViewElementSquare()
{

}

void CViewElementSquare::VPreRender()
{
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
}


void CViewElementSquare::VRender()
{
	//if (!loadShader("simple"))
	{
		//return;
	}
	// prepares the texture
	//_CrtDumpMemoryLeaks();
	//applyTexture("goku.bmp");
	applyTexture(m_textureId);

	// moves the rectangle to the correct location
	glTranslated(m_position.x, m_position.y, m_position.z);

	glBegin(GL_QUADS);
	// dimensions according to the given parameters
	/*
	       D---A
		   | 0 |
           C---B	
	*/
	glTexCoord2f(1, 0);
	glVertex3f((m_scale.x / 2.0), -(m_scale.y / 2.0), 0.0f); // A
	glTexCoord2f(1, 1);
	glVertex3f((m_scale.x / 2.0), (m_scale.y / 2.0), 0.0f); // B
	glTexCoord2f(0, 1);
	glVertex3f(-(m_scale.x / 2.0), (m_scale.y / 2.0), 0.0f); // C
	glTexCoord2f(0, 0);
	glVertex3f(-(m_scale.x / 2.0), -(m_scale.y / 2.0), 0.0f); // D
	glEnd();
	glDisable(GL_TEXTURE_2D);
    TInt32 err = glGetError();
	if (err != 0)
	{
		printf("glError glDisable=%d\n", err);
	}
	
}

void CViewElementSquare::VPostRender()
{
	glPopMatrix();
	TInt32 err = glGetError();
	if (err != 0)
	{
		printf("glError glPopMatrix=%d\n", err);
	}
}

void CViewElementSquare::applyTexture(string textId)
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
		printf("glError EnableTexture2D=%d\n", err);
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
CViewElementSquare::loadShader(string shaderId)
{
	// get texture from cache
	if (!CShaderManager::instance()->getShaderById(shaderId))
	{
		return false;
	}

	return true;
}
