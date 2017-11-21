#include "CTextManager.h"
#include "CGameCockpit.h"
#include "CProcessLoadTexture.h"
#include "gl/glut.h"
#include <time.h>
#ifdef _WIN_
#include "CWinMutex.h"
#endif
#include "CNoise.h"
#include "CTargaImage.h"

const char CTextManager::filenames[6][7] = {
	"lt.tga",
	"rt.tga",

	"dn.tga",
	"up.tga",

	"bk.tga",
	"ft.tga"
};


CTextManager* CTextManager::s_pInstance = NULL;

CTextManager*
CTextManager::instance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CTextManager();
		s_pInstance->OpenResourceFile();
		//s_pInstance->m_cacheDb.Init();
	}

	return s_pInstance;
}

void 
CTextManager::OpenResourceFile()
{
	m_textureFiles.VOpen();

	// add common textures
	// NOISE TEXTURE
	{
		glEnable(GL_TEXTURE_3D);
		GLuint NoiseTexture = -1;
		glGenTextures(1, &NoiseTexture);
		glBindTexture(GL_TEXTURE_3D, NoiseTexture);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Noise::CNoise::CreateNoise3D();

		// adds to the hashmap
		m_textures.insert(make_pair("3dnoise", NoiseTexture));

		glDisable(GL_TEXTURE_3D);
	}

	// 3D CUBE
	{
		glEnable(GL_TEXTURE_CUBE_MAP);
		GLuint CubeTexture = -1;
		glGenTextures(1, &CubeTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeTexture);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		char temp[256];
		for (int i = 0; i<6; ++i) {
			sprintf(temp, "./Resources/tex_cube/%s", filenames[i]);

			// targa image format (tga)
			CTargaImage img;

			if (!img.Load(temp)) {
				return;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img.GetWidth(), img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.GetImage());
			//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img.GetWidth(), img.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, img.GetImage());
			img.Release();
		}

		// adds to the hashmap
		m_textures.insert(make_pair("cubetexture", CubeTexture));

		glDisable(GL_TEXTURE_CUBE_MAP);
	}
}

void 
CTextManager::OnRemoveEvent(string removeItem)
{
	instance()->RemoveTexture(removeItem);
}

CTextManager::CTextManager() : 
	m_textureFiles("C:\\Users\\Vagner\\Documents\\Visual Studio 2015\\Projects\\GameAvatar\\GameAvatar\\Resources\\respack.zip", this->OnRemoveEvent)
	  //m_cacheDb(9, &m_textureFiles)
{
#ifdef _WIN_
	m_textureContentMapMutex = new CWinMutex();
#else
#error "no implementation for this platform"
#endif
	if (m_textureContentMapMutex != NULL)
	{
		m_textureContentMapMutex->createMutex("TextureContentMap");
	}
}

void 
CTextManager::LoadTexture(const string textId)
{
	// start loading measuring time
	clock_t start = clock();

	// cache missed - must reload it from resources db
	CResource resourceItem(textId);
	//shared_ptr<CResHandle> bytesStream = m_cacheDb.GetHandle(&resExample);
	Types::TByte* bytesStream = new Types::TByte[m_textureFiles.VGetResourceSize(resourceItem)];
	TInt32 status = m_textureFiles.VGetResource(resourceItem, bytesStream);
	//TByte* data = bytesStream->Buffer();	
	// status OK
	if (status == 0)
	{
		AddTextureContent(textId, bytesStream);
	}
	
	// time measurement
	printf(" loading tex [%s] %.2fms\n", textId.data(), (float)(clock() - start));
}

void CTextManager::BuildTexture()
{
	const int wrap = 0;
	// allocate a texture name
	GLuint GeneratedTexture = -1;
	// [BEGIN] CRITICAL AREA
	m_textureContentMapMutex->mutexLock();
	for (auto it = m_textureContentMap.begin(); !m_textureContentMap.empty();)
	{
		// whole data content of the image (texture)
		TByte* data = it->second;
		// retrieve header information
		// offset for start of bmp data
		TInt32 dataPos = *(int*)&(data[0x0A]);
		// image size in bytes
		TInt32 imageSize = *(int*)&(data[0x22]);
		// width in pixels
		TInt32 width = *(int*)&(data[0x12]);
		// height in pixels
		TInt32 height = *(int*)&(data[0x16]);
		// supports only 24bits and 32bits
		TInt32 numOfBytesPerPixel = 3;
		// checks if this image is 32 bits
		if (imageSize == (width * height * 4))
			numOfBytesPerPixel = 4;

		// then, allocate an indexer for new texture
		glGenTextures(1, &GeneratedTexture);
		// binds this texture as a 2D bitmap
		glBindTexture(GL_TEXTURE_2D, GeneratedTexture);
		int err = glGetError();
		if (err != 0)
		{
			printf("glError BindTexture=%d\n", err);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		// always good to get the error in case it happens
		err = glGetError();
		if (err != 0)
		{
			printf("glError Texture parameters=%d\n", err);
		}
		// data must be sent in BGRA order - must swap byte order of integer
		TByte* swapBuffer = new TByte[width*height*numOfBytesPerPixel];
		for (TInt32 x = 0; x < (width*height*numOfBytesPerPixel);)
		{
			if (numOfBytesPerPixel == 4)
			{
				swapBuffer[x] = data[x + dataPos + 2];
				swapBuffer[x + 1] = data[x + dataPos + 1];
				swapBuffer[x + 2] = data[x + dataPos + 0];
				swapBuffer[x + 3] = data[x + dataPos + 3];
				x += numOfBytesPerPixel;
			}
			if (numOfBytesPerPixel == 3)
			{
				swapBuffer[x] = data[x + dataPos + 2];
				swapBuffer[x + 1] = data[x + dataPos + 1];
				swapBuffer[x + 2] = data[x + dataPos];
				x += numOfBytesPerPixel;
			}
		}

		// build our texture mipmaps	
		if (numOfBytesPerPixel == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, swapBuffer);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, swapBuffer);
		}
		// release temporary buffer for swap
		delete[] swapBuffer;
		// checks for GL ERROR
		err = glGetError();
		if (err != 0)
		{
			printf("glError glTexImage2D=%d\n", err);
		}
		// adds to the hashmap
		m_textures.insert(make_pair(it->first, GeneratedTexture));

		// deletes allocated memory (do not allow it to leak)
		delete[] it->second;
		// deletes the object from the DS
		m_textureContentMap.erase(it);
	}
	// [END] CRITICAL AREA
	m_textureContentMapMutex->mutexUnlock();

}

void CTextManager::RemoveTexture(const string textId)
{
	TextureMap::iterator it = m_textures.find(textId);
	if (it == m_textures.end())
	{
		// this shouldn't happen - never, but if happens, trying 
		// to erase will cause an exception - so must quit method
		return;
	}

	glDeleteTextures(1, &(it->second));
	m_textures.erase(it);

	//_CrtDumpMemoryLeaks();
}

GLuint
CTextManager::getTextureById(string textId)
{
	// builds any texture it has to build first
	BuildTexture();
	// then try to find it in the textures map
	TextureMap::iterator result = m_textures.find(textId);
	if (result != m_textures.end())
	{
		return result->second;
	}

	// cache miss - then add this texture to the process list
	shared_ptr<CProcessLoadTexture> pProcess(new CProcessLoadTexture(textId));
	CGameCockpit::instance()->pushProcess(pProcess);

	// if it somehow failed, returns -1
	return -1;
}

void 
CTextManager::AddTextureContent(string textId, Types::TByte* data)
{
	m_textureContentMapMutex->mutexLock();
	m_textureContentMap.insert(make_pair(textId, data));
	m_textureContentMapMutex->mutexUnlock();
}

CTextManager::~CTextManager()
{
	while (!m_textures.empty()) 
	{		
		TextureMap::iterator it = m_textures.begin();
		glDeleteTextures(1, &(it->second));
		m_textures.erase(it);
	}		
}

