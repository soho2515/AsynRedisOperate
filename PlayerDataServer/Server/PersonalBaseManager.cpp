#include "PersonalBaseManager.h"

//PersonalBaseManager::PersonalBaseManager(void)
//{
//}
//
//PersonalBaseManager::~PersonalBaseManager(void)
//{
//}
//
//
//bool PersonalBaseManager::Instance()
//{
//	return true;
//}
//
//void PersonalBaseManager::Exit()
//{
//
//}
//
//PersonalBase* PersonalBaseManager::AllocNewPersonalBase()
//{
//	PersonalBase* pBase = new PersonalBase();
//	if (pBase != NULL)
//	{
//		return pBase;
//	}
//	return NULL;
//}
//
//bool PersonalBaseManager::ReadPersonalBaseFromDB(ID_t nPersonalBaseID)
//{
//	//TODO: ??? 应该是异步的
//	PersonalBase* pBase = AllocNewPersonalBase();
//	if (pBase)
//	{
//		pBase->load();
//		m_mapPersonalBase[nPersonalBaseID] = pBase;
//	}
//	else
//	{
//		return false;
//	}
//	return true;
//}
//
//void PersonalBaseManager::Update(float dt)
//{
//	std::map<ID_t, PersonalBase*>::iterator it = m_mapPersonalBase.begin();
//	for ( ; it!=m_mapPersonalBase.end(); ++it)
//	{
//		it->second->update(dt);
//	}
//}
//
//int PersonalBaseManager::GetPersonalBaseSize()
//{
//	return m_mapPersonalBase.size();
//}
//
//PersonalBase* PersonalBaseManager::GetPersonalBaseByID(ID_t nPersonalBaseID)
//{
//	if (m_mapPersonalBase.find(nPersonalBaseID) != m_mapPersonalBase.end())
//	{
//		return m_mapPersonalBase[nPersonalBaseID];
//	}
//
//	return NULL;
//}

PersonalBase* PersonalBaseManager::getPersonalBaseByGuid( Guid_t ownerGuid )
{
	__ENTER_FUNCTION__
		PersonalBaseMap::iterator it = m_mapPersonalBase.find(ownerGuid);
	if (it != m_mapPersonalBase.end())
	{
		return it->second;
	}
	return NULL;
	__LEAVE_FUNCTION__
}

PersonalBase* PersonalBaseManager::getFreePersonalBase( Guid_t ownerGuid )
{
	__ENTER_FUNCTION__
		PersonalBase* pPersonalBase = (PersonalBase *)GetFreeObj();
	if (pPersonalBase != NULL)
	{
		pPersonalBase->SetOwnerGUID(ownerGuid);
		m_mapPersonalBase[ownerGuid] = pPersonalBase;
	}
	return pPersonalBase;
	__LEAVE_FUNCTION__
}

void PersonalBaseManager::recyclePersonalBase( PersonalBase* p )
{
	__ENTER_FUNCTION__
		m_mapPersonalBase.erase(p->GetOwnerGUID());
	RecycleObj(p);
	__LEAVE_FUNCTION__
}

PersonalBase* PersonalBaseManager::readPersonalBaseFromDB( Guid_t nOwnerGuid )
{
	__ENTER_FUNCTION__
		// !! TO DO [12/7/2012 dujiang] 应该是异步的, 下一步要换成从DB中取
		PersonalBase* pBase = getFreePersonalBase(nOwnerGuid);
	if (pBase)
	{
		PersonalBaseInfo info;
		pBase->load(info);
		m_mapPersonalBase[nOwnerGuid] = pBase;
	}
	else
	{
		CacheLog_ERROR(LOG_ERROR, "can get more free base, current size:%d", m_mapPersonalBase.size());
		return NULL;
	}
	return pBase;
	__LEAVE_FUNCTION__
}

void PersonalBaseManager::removeVisitorFromConneciton( ClientConnection* pConnection )
{
	__ENTER_FUNCTION__
		typedef std::map<Guid_t, PersonalBase*>  PersonalBaseMap;
	std::map<Guid_t, PersonalBase*> m_mapPersonalBase;
	PersonalBaseMap::iterator it, end(m_mapPersonalBase.end());
	for (it = m_mapPersonalBase.begin(); it != end;){
		PersonalBase* pBase = it->second;
		if (pBase == NULL){
			m_mapPersonalBase.erase( it++ );
		}
		else{
			pBase->RemoveVisitorFromConnection(pConnection);
			++it;
		}
	}
	__LEAVE_FUNCTION__
}