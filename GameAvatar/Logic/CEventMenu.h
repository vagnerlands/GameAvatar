#ifndef _CEVENTMENUSTATE_H_
#define _CEVENTMENUSTATE_H_

#include "CCommonTypes.h"
#include "IEvent.h"
#include "ISocket.h"
#include <string>

using namespace Types;
using namespace std;

class CEventMenu : public IEvent {
public:
	CEventMenu()
	{

	}

	virtual ~CEventMenu();

	ISocket* GetKey()
	{
		return m_socket;
	}

	void SetKey(ISocket* socket)
	{
		m_socket = socket;
	}

	ESocketState GetState()
	{
		return m_state;
	}

	void SetState(ESocketState state)
	{
		m_state = state;
	}

	TInt32 GetOption()
	{
		return m_option;
	}

	void SetOption(TInt32 option)
	{
		m_option = option;
	}

	EEventType GetType()
	{
		return EEVENTTYPE_MENU_STATE;
	}

protected:
	// pointer to socket interface
	ISocket* m_socket;
	// current state
	ESocketState m_state;
	// selected option (shall be casted accordingly to the current state to reflect user selection)
	TInt32 m_option;

};

#endif
