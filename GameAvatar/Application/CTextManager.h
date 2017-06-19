#ifndef CTEXTMANAGER_H_
#define CTEXTMANAGER_H_

#include "CCommonTypes.h"
#include "CResHandle.h"
#include "IMutex.h"
#include <unordered_map>
#include <memory>
#include "gl/glut.h"

using namespace Types;
using namespace std;

class CTextManager 
{
public:
	static CTextManager* instance();
	void LoadTexture(const string textId);
	void RemoveTexture(const string textId);
	GLuint getTextureById(string textId);
	void AddTextureContent(string textId, shared_ptr<CResHandle> data);
	CTextManager::~CTextManager();

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(string removeItem);


private:
	CTextManager();
	// build texture, if any available
	void BuildTexture();
	// local hashmap built textures
	TextureMap m_textures;
	// local hashmap for textures to be generated
	TextureContentMap m_textureContentMap;
	// mutex for m_processes
	IMutex* m_textureContentMapMutex;

	// file content may be found here
	CResourceZipFile m_textureFiles;
	// cache database (allocated with fixed and known size)
	CResCache m_cacheDb;
	// local instance
	static CTextManager* s_pInstance;

};

#endif //CTEXTMANAGER_H_