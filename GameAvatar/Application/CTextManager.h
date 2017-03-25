#ifndef CTEXTMANAGER_H_
#define CTEXTMANAGER_H_

#include "CCommonTypes.h"
#include "CResHandle.h"
#include <unordered_map>
#include <memory>
#include "gl/glut.h"

using namespace Types;
using namespace std;

class CTextManager
{
public:
	static CTextManager* getInstance();
	void LoadTexture(const TByte* data, const string textId);
	void RemoveTexture(const string textId);
	GLuint getTextureById(string textId);
	CTextManager::~CTextManager();

private:
	CTextManager();
	TextureMap m_textures;
	CResourceZipFile m_textureFiles;
	CResCache m_cacheDb;
	static CTextManager* s_instance;

};

#endif //CTEXTMANAGER_H_