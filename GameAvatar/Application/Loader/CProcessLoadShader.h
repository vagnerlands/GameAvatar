#ifndef CPROCESSLOADSHADER_H_
#define CPROCESSLOADSHADER_H_

#include "CCommonTypes.h"
#include "IProcess.h"

using namespace Types;
using namespace std;

class CProcessLoadShader : public IProcess
{
public:
	CProcessLoadShader(string objectId)
	{
		m_objectId = objectId;
	}
	// for debug purposes
	virtual ProcessType VGetType();
	virtual void load();

};

#endif //CPROCESSLOADSHADER_H_