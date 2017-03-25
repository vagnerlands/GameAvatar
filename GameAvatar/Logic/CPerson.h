#ifndef _CPERSON_H_
#define _CPERSON_H_

#include "CCommonTypes.h"
#include "CRoom.h"

using namespace Types;

class CPerson {
public:
	CPerson(string user, bool isCharacterCreated, ECharacterGender gender, ECharacterRace race)
	{
		m_user = user;
		m_isCharacterCreated = isCharacterCreated;
		m_race = race;
		m_gender = gender;
		m_class = ECharacterClass::ECHARACTERCLASS_WARRIOR;
	}

	string getUser() const
	{
		return m_user;
	}

	void setUser(string user)
	{
		m_user = user;
	}

	ECharacterClass getClass()
	{
		return m_class;
	}

	void setClass(ECharacterClass charClass)
	{
		m_class = charClass;
	}

	// returns reference of the m_isCharacterCreated - so the 
	// caller may change its value
	bool* isCharacterCreated()
	{
		return &m_isCharacterCreated;
	}

	// attributes and other information

protected:
	// shall have a pointer to the room location where 
	CRoom* m_location;
	ISocket* m_socket;
	TInt32 m_life;
	TInt32 m_mana;
	TInt32 m_strength;

	string m_user;
	bool m_isCharacterCreated;
	ECharacterRace m_race;
	ECharacterGender m_gender;
	ECharacterClass m_class;

};

#endif
