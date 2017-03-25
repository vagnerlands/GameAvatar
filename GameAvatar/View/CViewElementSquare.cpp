#include "CHumanView.h"
#include "CViewElementSquare.h"

CViewElementSquare::CViewElementSquare(TInt32 posX, TInt32 posY, TInt32 width, TInt32 height)
{
	m_posX = posX;
	m_posY = posY;
	m_width = width;
	m_height = height;
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
	if (++ttt  <= 10000)
	{
		bindTexture("texAridTerrain.bmp");
	}
	else if (ttt <= 20000)
	{
		bindTexture("paper.bmp");
	}
	else
	{
		ttt = 0;
	}
	// moves the rectangle to the correct location
	glTranslated(m_posX, m_posY, 0.0);

	//glRotated(180, 0.0, 1.0, 0.0);
	//glRotated(45.0, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glColor4d(0.0, 0.5, 0.0, 0.5);	
	// dimensions according to the given parameters
	/*
	       D---A
		   | 0 |
           C---B	
	*/
	glTexCoord2f(0, 0);
	glVertex3f((m_width / 2.0), -(m_height / 2.0), 0.0f); // A
	glTexCoord2f(1, 0);
	glVertex3f((m_width / 2.0), (m_height / 2.0), 0.0f); // B
	glTexCoord2f(1, 1);
	glVertex3f(-(m_width / 2.0), (m_height / 2.0), 0.0f); // C
	glTexCoord2f(0, 1);
	glVertex3f(-(m_width / 2.0), -(m_height / 2.0), 0.0f); // D
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}

void CViewElementSquare::bindTexture(string textId)
{
	GLuint texture = CTextManager::getInstance()->getTextureById(textId);
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

	glBindTexture(GL_TEXTURE_2D, texture);
	if (err != 0)
	{
		printf("glError glBindTexture=%d\n", err);
	}
}
