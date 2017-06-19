#ifndef CMODELMANAGER_H_
#define CMODELMANAGER_H_

#include "CCommonTypes.h"
#include "CResHandle.h"
#include "IMutex.h"
#include <unordered_map>
#include <memory>
#include "gl/glut.h"

using namespace Types;
using namespace std;

class CModelManager
{
public:
/*
	struct membuf : std::streambuf
	{
		membuf(char* begin, char* end) {
			this->setg(begin, begin, end);
		}
	};
*/
	static CModelManager* instance();
	void LoadModel(const string modelId);
	void RemoveModel(const string modelId);
	bool getModelById(string modelId, SModelData& out);
	//void AddModelContent(string modelId, shared_ptr<CResHandle> data);
	CModelManager::~CModelManager();

	// external callback event in case a resource is deallocated
	static void OnRemoveEvent(string removeItem);


private:
	CModelManager();
	void AddModelContent(string modelId, TByte* bytesStream);
	// local hashmap built textures
	ModelMap m_models;
	// mutex for m_processes
	IMutex* m_pModelContentMapMutex;

	// file content may be found here
	CResourceZipFile m_modelFiles;
	// cache database (allocated with fixed and known size)
	CResCache m_cacheDb;
	// local instance
	static CModelManager* s_pInstance;

};

#endif //CMODELMANAGER_H_