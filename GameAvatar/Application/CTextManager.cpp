#include "CTextManager.h"
#include "gl/glut.h"


CTextManager* CTextManager::s_instance = NULL;

CTextManager*
CTextManager::getInstance()
{
	if (s_instance == NULL)
	{
		s_instance = new CTextManager();
		s_instance->m_cacheDb.Init();
	}

	return s_instance;
}

CTextManager::CTextManager()
	: m_textureFiles("C:\\Users\\Vagner\\Documents\\Visual Studio 2015\\Projects\\GameAvatar\\GameAvatar\\Resources\\respack.zip"),
	  m_cacheDb(3, &m_textureFiles)
{
	
}



void 
CTextManager::LoadTexture(const TByte* data, const string textId)
{
	// load a 256x256 RGB .RAW file as a texture

	const int wrap = 0;
	// allocate a texture name
	GLuint GeneratedTexture = -1;
	
	//for debug only
	//int dataPos    = *(int*)&(data[0x0A]);
	//int imageSize  = *(int*)&(data[0x22]);
	TInt32 width      = *(int*)&(data[0x12]);
	TInt32 height     = *(int*)&(data[0x16]);

	// checks if the mentioned texture was already generated
	TextureMap::iterator it = m_textures.find(textId);
	if (it != m_textures.end())
	{
		return;
	}

	// then, allocate an indexer for new texture
    glGenTextures( 1, &GeneratedTexture);

    // binds this texture as a 2D bitmap
    glBindTexture( GL_TEXTURE_2D, GeneratedTexture );
	int err = glGetError();
	if (err != 0)
	{
		printf("glError BindTexture=%d\n", err);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	err = glGetError();
	if (err != 0)
	{
		printf("glError Texture parameters=%d\n", err);
	}


    // build our texture mipmaps	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	err = glGetError();
	if (err != 0)
	{
		printf("glError glTexImage2D=%d\n", err);
	}

	m_textures.insert(make_pair(textId, GeneratedTexture));
}

void CTextManager::RemoveTexture(const string textId)
{
	TextureMap::iterator it = m_textures.find(textId);
	if (it == m_textures.end())
	{
		return;
	}

	glDeleteTextures(1, &(it->second));
	m_textures.erase(it);
}

GLuint
CTextManager::getTextureById(string textId)
{
	TextureMap::iterator result = m_textures.find(textId);
	if (result != m_textures.end())
	{
		return result->second;
	}
	// cache missed - must reload it from resources db
	CResource resExample(textId);
	shared_ptr<CResHandle> bytesStream = m_cacheDb.GetHandle(&resExample);
	int size = bytesStream->GetSize();
	char* buf = bytesStream->Buffer();
	// perform the load texture (binds it to opengl memory)
	LoadTexture(buf, textId);
	// try once again - this should never ever fail
	result = m_textures.find(textId);
	if (result != m_textures.end())
	{
		return result->second;
	}
	// if it somehow failed, returns -1
	return -1;
}

CTextManager::~CTextManager()
{
	for (auto it = m_textures.begin(); it != m_textures.end();) {		
		glDeleteTextures(1, &(it->second));
		m_textures.erase(it);
	}		
}