#ifndef _CTHREADHOLDER_H_
#define _CTHREADHOLDER_H_

#include <string>
#include <iostream>
#include "CCommonTypes.h"
#include "IThread.h"
#include <unordered_map>

using namespace Types;
using namespace std;

class CThreadHolder {
public:
	static CThreadHolder* instance();
	void destroyThread(string threadId);
	void registerThread(string thName, void* thEntry);
private:
	CThreadHolder();
	~CThreadHolder();
	static CThreadHolder* s_pInstance;

	unordered_map<string, IThread*> m_threadMap;
};

#endif //_CTHREADHOLDER_H_
