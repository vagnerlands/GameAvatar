#include "CSocketHolder.h"
#include "CThreadHolder.h"
#ifdef _WIN_
#include "CWinMutex.h"
#endif

CSocketHolder::CSocketHolder() : m_sockedDBMutex(NULL)
{
#ifdef _WIN_
	m_sockedDBMutex = new CWinMutex();
#endif

	if (m_sockedDBMutex == NULL)
	{
		cout << "<!> Critical issue while allocating resources, program is being terminated!" << endl;
		exit(1);
	}

	m_sockedDBMutex->createMutex("socketDB");
}

CSocketHolder::~CSocketHolder()
{
	// loops through all open sockets and close them all
	// Iterate and print keys and values of unordered_map
	for (const auto& socketItem : m_sockedDB) {
		cout << "Forcing connect Key:[" << socketItem.first << "] to close !" << endl;
		ISocket* condemed = socketItem.second;
		delete condemed;
		condemed = NULL;
	}
}

void 
CSocketHolder::addSocket(string user, ISocket* userSocket)
{
	// protects with MUTEX
	m_sockedDBMutex->mutexLock();
	// check if this ip is already registered
	unordered_map<string, ISocket*>::const_iterator checkIterator = m_sockedDB.find(user);
	if (checkIterator != m_sockedDB.end())
	{
		cout << "Duplicated connection from Key:[" << checkIterator->first << "] - first connection will be closed!" << endl;
		CAnsiString quitMsg;
		quitMsg.resetFormat();
		quitMsg.setForegroundColor(EANSICOLOR_YELLOW);
		quitMsg += "\n\r\n\rDuplicated connection detected, your connection is being halted!";
		checkIterator->second->write(quitMsg.getData().c_str(), quitMsg.size());
		ISocket* condemed = checkIterator->second;
		delete condemed;
		condemed = NULL;
		m_sockedDB.erase(user);
	}
	// creates a pair for user/userSocket
	m_sockedDB.insert({ user, userSocket });
	// Releases MUTEX
	m_sockedDBMutex->mutexUnlock();
}

bool
CSocketHolder::dropSocket(string user)
{
	// creates a pair for user/userSocket
	m_sockedDB[user]->closeSocket();
	bool retVal = m_sockedDB.erase(user);

	return retVal;
}

void
CSocketHolder::readIncomingMsgs()
{
	for (const auto& socketItem : m_sockedDB) {
		char tempBuf[1025];
		int bufSize;
		int iRes = socketItem.second->read(tempBuf, &bufSize);

		// error code - lost connection;
		if (iRes == 100)
		{
			socketItem.second->recycle();
			cout << "ERROR READING " << socketItem.first << " Code " << iRes << endl;
		}
	}

}

void CSocketHolder::executeCommands()
{
	// intentionally - only one command per socket in this cycle will be parsed
	// we don't want cheaters to take advantage
	for (const auto& socketItem : m_sockedDB) {
		// general 
		CICommand *newCommand = NULL;
		// checks if null before using (memory violation)
		if (socketItem.second != NULL)
		{
			newCommand = socketItem.second->CreateCommand();
		}

		if (newCommand != NULL)
		{
			// for consult - if needed
			newCommand->setKey(socketItem.first);
			// to update the status - send messages to client
			newCommand->setSocket(socketItem.second);
			// executes it immediatelly
			newCommand->execute();
			// deletes the allocated memory to avoid mem leak
			delete newCommand;
			newCommand = NULL;
		}
	}
}

TInt32 
CSocketHolder::removeCondemedSockets()
{
	TInt32 retVal = 0;

	// protects with MUTEX
	m_sockedDBMutex->mutexLock();

	for (auto socketItem = m_sockedDB.begin(); socketItem != m_sockedDB.end(); )
	{
	//for (const auto& socketItem : m_sockedDB) {
		if (socketItem->second != NULL)
		{
			if (socketItem->second->recycleStatus())
			{
				ISocket* condemed = socketItem->second;
				delete condemed;
				condemed = NULL;
				// removes the item from the DS
				m_sockedDB.erase(socketItem++);
			} 
			else
			{
				++socketItem;
			}
		}
	}

	// release MUTEX
	m_sockedDBMutex->mutexUnlock();

	return retVal;
}

void 
CSocketHolder::onConnectionEvent()
{
	#ifdef _WIN_
	ISocket* inSocket = new CWinSocket();
    #else
	// not implemented for other platforms
	ISocket* inSocket = NULL;
	#endif

	if (inSocket == NULL)
	{
		cout << "<!> Failed to allocate Socket!" << endl;
		return;
	}
	// blocking action - stays here till a new user connects
	inSocket->startListenSocket();

	CAnsiString welcomeMsg; 
	// reset whatever was set previously
	welcomeMsg.resetFormat();
	welcomeMsg.setForegroundColor(Types::EANSICOLOR_CYAN);
	welcomeMsg += "Welcome do Avatar MUD\r\n";
	welcomeMsg.setForegroundColor(Types::EANSICOLOR_WHITE);
	welcomeMsg += "---------------------\r\n";
	welcomeMsg.setForegroundColor(Types::EANSICOLOR_MAGENTA);
    welcomeMsg += "   ___            __             __  _____  _____ \r\n";
    welcomeMsg += "  / _ |_  _____ _/ /____ _____  /  |/  / / / / _ \\\r\n";
    welcomeMsg += " / __ | |/ / _ `/ __/ _ `/ __/ / /|_/ / /_/ / // /\r\n";
    welcomeMsg += "/_/ |_|___/\\_,_/\\__/\\_,_/_/   /_/  /_/\\____/____/ \r\n";
    welcomeMsg += "                                                  \r\n";
    
	welcomeMsg.setForegroundColor(Types::EANSICOLOR_WHITE);
    welcomeMsg += "\n---------------------\r\n";
	welcomeMsg.setForegroundColor(Types::EANSICOLOR_CYAN);
	welcomeMsg += "Developed by Avatar - ";
	welcomeMsg.setBackgroundColor(Types::EANSICOLOR_BLUE);
	welcomeMsg.setForegroundColor(Types::EANSICOLOR_YELLOW);
	welcomeMsg.setBold();
	welcomeMsg += "avatarch@gmail.com \r\n";
	welcomeMsg.resetFormat();

	welcomeMsg.setForegroundColor(Types::EANSICOLOR_CYAN);
	welcomeMsg += "\r\n> Please, insert your user: ";
	welcomeMsg.setBackgroundColor(Types::EANSICOLOR_RED);
	inSocket->write(welcomeMsg.getData().c_str(), welcomeMsg.getData().length());

	// invalid socket
	if (strcmp(inSocket->getClientIP(), "205.205.205.205") != 0) 
	{
		// once someone connects - this socket shall be sent to Login process
		// which will be treated in the main thread...
		// in any case, the ID of this connection shall be the connection IP
		addSocket(inSocket->getClientIP(), inSocket);
	} 
	else
	{
		delete inSocket;
	}


	// the process finishes here and a new socket shall be allocated in the next iteration
	// and should remain in startListenSocket() till a new user performs a connection

}

