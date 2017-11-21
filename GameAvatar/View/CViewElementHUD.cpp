#include "CHumanView.h"
#include "CViewElementHUD.h"
#include "CModelManager.h"
#include "CShaderManager.h"
#include "CFonts.h"

CViewElementHUD::CViewElementHUD(Types::CPoint m_start, Types::CPoint m_end, string hudName)
{	
	m_position.x = m_start.x + ((m_end.x - m_start.x) / 2);
	m_position.y = m_start.y + ((m_end.y - m_start.y) / 2);
	m_position.z = 0.0f;
	m_scale.x = abs(m_start.x - m_end.x);
	m_scale.y = abs(m_start.y - m_end.y);
	m_modelId = hudName;
	m_msgList = "String de teste limp bizkit \nEra uma vez a historia de um leao que gostava de tut, somente tut\nNao de carninha e nem de leitinho, apenas Tut...";
}

CViewElementHUD::~CViewElementHUD()
{

}

void CViewElementHUD::VPreRender()
{
	glPushMatrix();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_TEXTURE_2D);

	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_ZERO, GL_SRC_ALPHA);
}


void CViewElementHUD::VRender()
{
	// apply texture - if found, Texture will be enabled
	applyTexture("board.bmp");
	glPushMatrix();
	glColor4f(0.0F, 0.0F, 0.0F, m_transparency);
	glTranslatef(m_position.x, m_position.y, 0);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 0);
	glVertex3f((m_scale.x / 2.0), -(m_scale.y / 2.0), 0.0f); // A
	glTexCoord2f(1, 1);
	glVertex3f((m_scale.x / 2.0), (m_scale.y / 2.0), 0.0f); // B
	glTexCoord2f(0, 1);
	glVertex3f(-(m_scale.x / 2.0), (m_scale.y / 2.0), 0.0f); // C
	glTexCoord2f(0, 0);
	glVertex3f(-(m_scale.x / 2.0), -(m_scale.y / 2.0), 0.0f); // D
	glEnd();
	glPopMatrix();

	glPushMatrix();
	SFontAttributes fontAttr = CFonts::instance()->GetFont("mono8");
	VRenderBitmap(m_position.x - m_scale.x/2 + 5, 
		m_position.y + m_scale.y / 2 - 15, 
		fontAttr.m_fontWidth, 
		m_scale.x / fontAttr.m_fontWidth - 1, 
		m_scale.y / fontAttr.m_fontHeight - 1,
		fontAttr.m_fontId, 
		m_msgList.data());
	glPopMatrix();
}

void
CViewElementHUD::VRenderBitmap(TFloat x, TFloat y, TInt32 fontSize, TInt32 maxWidth, TInt32 maxHeight, void *font, const TByte *string)
{
	const char *c;
	// set position to start drawing fonts
	glDisable(GL_BLEND);

	//static int px = 0;
	//static int py = 0;
	glRasterPos2f(x, y);
	//if (px-- < -100) px = 0;
	//if (py-- < -100) py = 0;
	//printf("px %d, py %d\n", px, py);
	glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
	// loop all the characters in the string
	int charCount = 0;
	for (c = string; *c != '\0'; c++) {
		// checks if it's a New Line char indication
		if (*c == '\n')
		{
			// jumps to the next line
			charCount += maxWidth - (charCount % maxWidth) - 1;
		}
		else
		{
			glutBitmapCharacter(font, *c);
		}
		// if char count reached the boarder, must perform CR and NL
		if ((++charCount % maxWidth) == 0)
		{
			glRasterPos2f(x, y - ((fontSize+5)*(charCount / maxWidth)));
		}
	}
}

void CViewElementHUD::VPostRender()
{

	TInt32 err = glGetError();
	if (err != 0)
	{
		printf("glError Disabling states Model = %d\n", err);
	}

	// disable texture 	
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void CViewElementHUD::applyTexture(string textId)
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
CViewElementHUD::loadModel(string modelId)
{
	// get texture from cache
	if (!CModelManager::instance()->getModelById(modelId, m_data))
	{
		return false;
	}

	return true;
}

bool 
CViewElementHUD::loadShader(string shaderId)
{
	// get texture from cache
	if ((m_pProgramShader = CShaderManager::instance()->getShaderById(shaderId)) == 0)
	{
		return false;
	}

	return true;
}