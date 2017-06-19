#ifndef CPROCESSLOADMODEL_H_
#define CPROCESSLOADMODEL_H_

#include "CCommonTypes.h"
#include "IProcess.h"

using namespace Types;
using namespace std;

class CProcessLoadModel : public IProcess
{
public:
	CProcessLoadModel(string objectId)
	{
		m_objectId = objectId;
	}
	// for debug purposes
	virtual ProcessType VGetType();
	virtual void load();

};

#endif //CPROCESSLOADMODEL_H_