#ifndef IPROCESS_H_
#define IPROCESS_H_

#include "CCommonTypes.h"

using namespace Types;
using namespace std;

class IProcess
{
public:

	// for debug purposes
	virtual ProcessType VGetType() = 0;
	virtual void load() = 0;
	virtual ~IProcess()
	{
		// no implementation
	}

protected:
	string m_objectId;

};

#endif //IPROCESS_H_