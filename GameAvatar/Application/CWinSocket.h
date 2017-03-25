#ifndef _CWINSOCKET_H_
#define _CWINSOCKET_H_

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <queue>
#include "CCommonTypes.h"
#include "CCommandLoginUserName.h"
#include "CCommandLoginMenu.h"
#include "CCommandLoginClassMenu.h"
#include "CCommandLoginRaceMenu.h"
#include "CCommandLoginGenderMenu.h"
#include "CICommand.h"
#include "ISocket.h"

#include "CAnsiString.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace Types;

class CWinSocket : public ISocket {
public:

	CWinSocket();
	~CWinSocket();
	// Returns the status of a socket connection
	// 0 succeeded;
	// any other value is fail
	ESocketConnectionStatus startListenSocket();
	
	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// content of buffer till it's size will be copied
	TInt32 write(const TByte* buffer, TInt32 const size);

	// return value 0 is okay
	// return value isn't 0 - connection problem, delete this socket
	//  to avoid memory leak
	// the buffer will be copied to Byte* pointer
	TInt32 read(TByte* buffer, TInt32* size);

	void closeSocket();

	void invalidateSocket();

	TUInt16 getClientPort();
	TByte* getClientIP();

	CICommand* CreateCommand();

	string setState(ESocketState nextState);

	ESocketState getState();

	// non used
	void clearReadBuffer();
	// non used
	char* const getCommand();

private:
	// parses the m_readBuffer and insert commands in m_commandQ
	void parseReadBuffer();
	// maximum buffer size for read messages
	static const TUInt32 s_MAX_BUFFER_SIZE = 1024;
	// client socket 
	SOCKET m_clientSocket;
	// intentionally static
	// one listener shall be allocated per Port
	static SOCKET m_listenSocket;
	// details about this client socket 
	sockaddr_in m_clientInfo;

	// input buffer for commands
	char m_readBuffer[s_MAX_BUFFER_SIZE];
	// read buffer last index
	TInt32 m_rbli;

	// this socket status
	ESocketState m_state;

	// command Queue to be performed
	queue<string> m_commandQ;

};

inline void
CWinSocket::invalidateSocket()
{
	m_clientSocket = INVALID_SOCKET;
	WSACleanup();
}

inline void
CWinSocket::closeSocket()
{
	closesocket(m_clientSocket);
	m_clientSocket = INVALID_SOCKET;
}

inline TUInt16
CWinSocket::getClientPort()
{
	return ntohs(m_clientInfo.sin_port);
}

inline TByte*
CWinSocket::getClientIP()
{
	return inet_ntoa(m_clientInfo.sin_addr);
}

inline void 
CWinSocket::clearReadBuffer()
{
	for (int i = 0; i < s_MAX_BUFFER_SIZE; i++) m_readBuffer[i] = 0;
	m_rbli = 0;
}

inline char* const
CWinSocket::getCommand()
{
	if (strlen(m_readBuffer) > s_MAX_BUFFER_SIZE)
	{
		cout << "Buffer overflow detected for " << getClientIP() << endl;
		m_readBuffer[s_MAX_BUFFER_SIZE - 1] = 0;
	}

	return m_readBuffer;
}

inline ESocketState 
CWinSocket::getState()
{
	return m_state;
}


#endif //_CWINSOCKET_H_
