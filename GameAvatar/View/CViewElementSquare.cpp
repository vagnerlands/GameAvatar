#include "CHumanView.h"
#include "CViewElementSquare.h"

CViewElementSquare::CViewElementSquare(TFloat posX, TFloat posY, TFloat width, TFloat height)
{
	m_posX = posX;
	m_posY = posY;
	m_width = width;
	m_height = height;
	m_volume = 1.0f;
}

CViewElementSquare::~CViewElementSquare()
{

}

void CViewElementSquare::VPreRender()
{
	glPushMatrix();
}


void CViewElementSquare::VRender()
{
	static int ttt = 0;	
	// prepares the texture
	//_CrtDumpMemoryLeaks();
	//applyTexture("goku.bmp");
	applyTexture("texSky.bmp");
	/*
	if (++ttt  <= 5000)
	{
		applyTexture("goku.bmp");
	}
	else if (ttt <= 10000)
	{
		applyTexture("texAridTerrain.bmp");
	}
	
	else if (ttt <= 15000)
	{
		applyTexture("orc.bmp");
	}
	
	else if (ttt <= 20000)
		
	{
		applyTexture("paladin.bmp");
	}
	
	else
	{
		ttt = 0;
	}
	*/
	// moves the rectangle to the correct location
	glTranslated(m_posX, m_posY, 0.0);

	//glRotated(180, 0.0, 1.0, 0.0);
	//glRotated(45.0, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	//glColor4d(1.0, 1.0, 1.0, 1.0);	
	// dimensions according to the given parameters
	/*
	       D---A
		   | 0 |
           C---B	
	*/
	glTexCoord2f(1, 0);
	glVertex3f((m_width / 2.0), -(m_height / 2.0), 0.0f); // A
	glTexCoord2f(1, 1);
	glVertex3f((m_width / 2.0), (m_height / 2.0), 0.0f); // B
	glTexCoord2f(0, 1);
	glVertex3f(-(m_width / 2.0), (m_height / 2.0), 0.0f); // C
	glTexCoord2f(0, 0);
	glVertex3f(-(m_width / 2.0), -(m_height / 2.0), 0.0f); // D
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
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
	/*
	glEnable(GL_BLEND); 
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);
	*/
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
