#ifndef _IRESOURCEFILE_H_
#define _IRESOURCEFILE_H_

//#include "CResource.h"

class IResourceFile {
	friend class CResource;
public:
	virtual bool VOpen() = 0;
	virtual int VGetResourceSize(const CResource &r) = 0;
	virtual int VGetResource(const CResource &r, char* buffer) = 0;
};

#endif 
