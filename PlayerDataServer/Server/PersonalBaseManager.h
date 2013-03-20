#pragma once

#include "ServerDefine.h"
#include "PersonalBase.h"

//class PersonalBaseManager : public Singleton<PersonalBaseManager>
//{
//public:
//	PersonalBaseManager(void);
//	~PersonalBaseManager(void);
//
//	bool Instance();
//	void Exit();
//
//	PersonalBase* AllocNewPersonalBase();
//	bool ReadPersonalBaseFromDB(ID_t nPersonalBaseID);
//	void Update(float dt);
//
//	int GetPersonalBaseSize();
//	PersonalBase* GetPersonalBaseByID(ID_t nPersonalBaseID);
//
//private:
//	std::map<ID_t, PersonalBase*> m_mapPersonalBase;
//
//};

#include "ObjManager.h"
#include "PersonalBase.h"

class PersonalBaseManager : public  ObjManager<PersonalBase, PLAYER_OBJ_ID_BASE>, public Singleton<PersonalBaseManager>
{
public:
	PersonalBase*	readPersonalBaseFromDB(Guid_t nOwnerGuid);

	PersonalBase*	getPersonalBaseByGuid(Guid_t ownerGuid);
	PersonalBase*	getFreePersonalBase(Guid_t ownerGuid);
	void			recyclePersonalBase(PersonalBase* p);

	void			removeVisitorFromConneciton(ClientConnection* pConnection);

private:
	typedef std::map<Guid_t, PersonalBase*>  PersonalBaseMap;
	std::map<Guid_t, PersonalBase*> m_mapPersonalBase;
};
