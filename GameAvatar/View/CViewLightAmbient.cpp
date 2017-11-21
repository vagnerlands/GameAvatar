#include "CHumanView.h"
#include "CViewLightAmbient.h"

CViewLightAmbient::CViewLightAmbient(TFloat posX, TFloat posY, TFloat width, TFloat height)
{
	m_position.x = posX;
	m_position.y = posY;
	m_position.z = 0.0f;
	m_scale.x = width;
	m_scale.y = height;
	m_scale.z = 1.0f;
}


void CViewLightAmbient::VPreRender()
{
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
}


void CViewLightAmbient::VRender()
{
	// prepares the texture
	//_CrtDumpMemoryLeaks();
	applyTexture("suntex.bmp");

	// light set-up
	glEnable(GL_LIGHT1);

	//Add directed light
	GLfloat lightColor1[] = { 0.2f, 0.8f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
	GLfloat ambColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)

	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor1);
	GLfloat lightPos[] = { m_position.x, m_position.y, m_position.z, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambColor);

	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


	// moves the rectangle to the correct location
	//glTranslated(m_position.x, m_position.y, m_position.z);

	glBegin(GL_QUADS);
	// dimensions according to the given parameters
	/*
	       D---A
		   | 0 |
           C---B	
	*/

	glTexCoord2f(1, 0);
	glVertex3f(
		m_position.x + (m_scale.x / 2.0),
		m_position.y - (m_scale.y / 2.0),
		m_position.z); // A
	glTexCoord2f(1, 1);
	glVertex3f(
		m_position.x + (m_scale.x / 2.0),
		m_position.y + (m_scale.y / 2.0),
		m_position.z); // B
	glTexCoord2f(0, 1);
	glVertex3f(
		m_position.x - (m_scale.x / 2.0),
		m_position.y + (m_scale.y / 2.0),
		m_position.z); // C
	glTexCoord2f(0, 0);
	glVertex3f(
		m_position.x - (m_scale.x / 2.0),
		m_position.y - (m_scale.y / 2.0),
		m_position.z); // D
	glEnd();
}

void CViewLightAmbient::VPostRender()
{
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	TInt32 err = glGetError();
	if (err != 0)
	{
		printf("glError glPopMatrix=%d\n", err);
	}
}


void CViewLightAmbient::applyTexture(string textId)
{
	// get texture from cache
	GLuint texture = CTextManager::instance()->getTextureById(textId);
	// requested texture was not found (the requested texture was put in a process queue and in next request it may be there)
	if (texture == -1)
	{
		return;
	}

	TInt32 err = glGetError();
	if (err != 0)
	{
		printf("glError Before=%d\n", err);
	}
	glEnable(GL_TEXTURE_2D);
	err = glGetError();
	if (err != 0)
	{
		printf("glError EnableTexture2D=%d\n", err);
	}
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*
	glEnable(GL_BLEND); 
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);
	*/
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
