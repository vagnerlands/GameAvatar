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
	static CTextManager* instance();
	void LoadTexture(const TByte* data, const string textId);
	void RemoveTexture(const string textId);
	GLuint getTextureById(string textId);
	CTextManager::~CTextManager();

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(string removeItem);

private:
	CTextManager();
	TextureMap m_textures;
	CResourceZipFile m_textureFiles;
	CResCache m_cacheDb;
	static CTextManager* s_pInstance;

};

#endif //CTEXTMANAGER_H_