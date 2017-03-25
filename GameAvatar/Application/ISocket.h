#ifndef ISOCKET_H_
#define ISOCKET_H_

#include "CCommonTypes.h"

using namespace std;
using namespace Types;

class CICommand;

class ISocket {
public:
	virtual ~ISocket() {

	}
	// Returns the status of a socket connection
	// 0 succeeded;
	// any other value is fail
	virtual ESocketConnectionStatus startListenSocket() = 0;

	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// content of buffer till it's size will be copied
	virtual TInt32 write(const TByte* buffer, TInt32 const size) = 0;

	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// the buffer will be copied to Byte* pointer
	virtual TInt32 read(TByte* buffer, TInt32* size) = 0;

	virtual  void closeSocket() = 0;

	virtual void invalidateSocket() = 0;

	virtual TUInt16 getClientPort() = 0;
	virtual TByte* getClientIP() = 0;

	virtual CICommand* CreateCommand() = 0;

	virtual string setState(ESocketState nextState) = 0;

	// non used
	virtual void clearReadBuffer() = 0;
	// non used
	virtual char* const getCommand() = 0;
	
	// set socket to recyclation
	void recycle()
	{
		m_recycle = true;
	}
	// get recyclation status from socket
	bool recycleStatus()
	{
		return m_recycle;
	}

protected:
	// shall be removed from DS
	bool m_recycle;
};

#endif
