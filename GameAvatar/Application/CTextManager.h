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
	void OpenResourceFile();
	void LoadTexture(const string textId);
	void RemoveTexture(const string textId);
	GLuint getTextureById(string textId);
	void AddTextureContent(string textId, Types::TByte* data);
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

	// resource database - TEXTURES
	CResourceZipFile m_textureFiles;

	// resource database
	//CResourceZipFile m_texture

	// cache database (allocated with fixed and known size)
	//CResCache m_cacheDb;
	// local instance
	static CTextManager* s_pInstance;

	static const char filenames[6][7];

};

#endif //CTEXTMANAGER_H_