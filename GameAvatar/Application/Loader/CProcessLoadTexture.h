#ifndef CPROCESSLOADTEXTURE_H_
#define CPROCESSLOADTEXTURE_H_

#include "CCommonTypes.h"
#include "IProcess.h"

using namespace Types;
using namespace std;

class CProcessLoadTexture : public IProcess
{
public:
	CProcessLoadTexture(string objectId)
	{
		m_objectId = objectId;
	}
	// for debug purposes
	virtual ProcessType VGetType();
	virtual void load();

};

#endif //CPROCESSLOADTEXTURE_H_