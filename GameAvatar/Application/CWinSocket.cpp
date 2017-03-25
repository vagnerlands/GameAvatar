#include "CWinSocket.h"

SOCKET CWinSocket::m_listenSocket = INVALID_SOCKET;

CWinSocket::CWinSocket()
{
	m_clientSocket = INVALID_SOCKET;
	m_state = ESOCKETSTATE_LOGIN;
	clearReadBuffer();
	m_recycle = false;
}

CWinSocket::~CWinSocket()
{
	// temporary variables
	int iResult;
	// shutdown the connection since we're done
	iResult = shutdown(m_clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
	} 
	else
	{
		// cleanup
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
	}
}

ESocketConnectionStatus
CWinSocket::startListenSocket()
{

	// temporary data for connection
	// -----
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	WSADATA wsaData;
	int iResult;
	// -----

	if (m_listenSocket == INVALID_SOCKET)
	{
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) 
		{
			printf("WSAStartup failed with error: %d\n", iResult);
			invalidateSocket();
			return ESOCKETCONNECTION_FAIL;
		}

		// zeroise this address of hints till the size of hints
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, (PCSTR)Types::s_GAME_CONNECTION_PORT, &hints, &result);
		if (iResult != 0) 
		{
			printf("getaddrinfo failed with error: %d\n", iResult);
			invalidateSocket();
			return ESOCKETCONNECTION_FAIL;
		}

		// blocking operation - make sure this is being called in a thread
		m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		// Setup the TCP listening socket
		iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(m_listenSocket);
			invalidateSocket();
			return ESOCKETCONNECTION_FAIL;
		}

		freeaddrinfo(result);
	}

	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) 
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(m_listenSocket);
		invalidateSocket();
		return ESOCKETCONNECTION_FAIL;
	}

	// Accept a client socket
	int size = sizeof(m_clientInfo);
	m_clientSocket = accept(m_listenSocket, (sockaddr*)&m_clientInfo, &size);
	if (m_clientSocket == INVALID_SOCKET) 
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(m_listenSocket);
		invalidateSocket();
		return ESOCKETCONNECTION_FAIL;
	}

	// non-blocking client socket
	u_long iMode = 1;
	ioctlsocket(m_clientSocket, FIONBIO, &iMode);

	return ESOCKETCONNECTION_OK;

}

TInt32 
CWinSocket::write(const TByte* buffer, TInt32 const size)
{
	// temporary variable
	int iSendResult;
	// Echo the buffer back to the sender
	iSendResult = send(m_clientSocket, buffer, size, 0);
	if (iSendResult == SOCKET_ERROR) 
	{
		printf("Connection was lost: %d\n", WSAGetLastError());
		closesocket(m_clientSocket);
		// error
		return 1;
	}
	// message sent
	return 0;
}


// incoming buffer
// size in bytes
TInt32
CWinSocket::read(TByte* buffer, TInt32 *size)
{
	// read the socket and place content in buffer*
	// *size shall hold the amount of bytes read
	*size = recv(m_clientSocket, buffer, strlen(buffer), 0);
	if (*size > 0)
	{
		if ((m_rbli + *size) < (s_MAX_BUFFER_SIZE - 1))
		{
			memcpy(&m_readBuffer[m_rbli], buffer, *size);
			m_rbli += *size;
			m_readBuffer[m_rbli + 1] = 0;
		}

		return 0;
	}
	else if ((*size == 0) || (m_clientSocket == INVALID_SOCKET))
	{
		printf("Connection was closed\n");
		return 100;
	} 
	else
	{
		// No data available
	}
	
	// insert commands in the m_commandQ
	parseReadBuffer();

	// message sent
	return 0;
}

void
CWinSocket::parseReadBuffer()
{
	// sequential chars \r\n were found - must continue trying to parse till no
	// more \r\n are found
	bool found = true;
	// main parsing loop
	while (found)
	{
		found = false;
		// Search for \r\n commands (enter)
		// and add these commands in the m_commandQ
		for (TInt32 i = 0; i < m_rbli; i++)
		{
			// if an [enter] key was pressed
			if ((m_readBuffer[i] == '\r') && (m_readBuffer[i + 1] == '\n'))
			{
				// transaction buffer
				char copyBuf[s_MAX_BUFFER_SIZE];
				// moves the command to this buffer
				memcpy(copyBuf, m_readBuffer, i);
				// set null terminator
				copyBuf[i] = 0;
				// explicitly created to avoid use of new operator
				string newCmd(copyBuf);
				// add this command to the command queue
				m_commandQ.push(newCmd);
				// and copies the remaining items to the current buffer
				// if there is a new command (#10 separator)
				// this shall be parsed in the next cycle - such as 1 command per time
				memcpy(m_readBuffer, &m_readBuffer[i + 2], m_rbli - i - 2);
				// i + 2 to include the new line feed (\n) in the string
				m_rbli -= (i + 2);
				// do not allow negative numbers - memory violation for sure
				if (m_rbli < 0)
					m_rbli = 0;
				// set this variable to true so the process will run once again
				// for this socket and parse again the input string 
				found = true;
			}
		}
	}
}

CICommand* 
CWinSocket::CreateCommand()
{
	CICommand* retVal = NULL;

	if (m_commandQ.empty()) return retVal;

	switch (m_state)
	{
		case ESOCKETSTATE_LOGIN:
			retVal = new CCommandLoginUserName(m_commandQ.front());
			m_commandQ.pop();
			break;
		case ESOCKETSTATE_LOGIN_MENU:
			retVal = new CCommandLoginMenu(m_commandQ.front());
			m_commandQ.pop();
			break;
		case ESOCKETSTATE_CREATE_NEW_CHAR_CLASS:
			retVal = new CCommandLoginClassMenu(m_commandQ.front());
			m_commandQ.pop();
			break;
		case ESOCKETSTATE_CREATE_NEW_CHAR_GENDER:
			retVal = new CCommandLoginGenderMenu(m_commandQ.front());
			m_commandQ.pop();
			break;
		case ESOCKETSTATE_CREATE_NEW_CHAR_RACE:
			retVal = new CCommandLoginRaceMenu(m_commandQ.front());
			m_commandQ.pop();
			break;
		default:
			break;
	}

	return retVal;
}

string CWinSocket::setState(ESocketState nextState)
{
	// should check validity before accepting
	m_state = nextState;

	return "";

}

