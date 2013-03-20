#include "DBCommandCallBackPool.h"
#include "DBCommandCallbackFactoryManager.h"

unsigned int DBCommandCallBackPool::AddDBCommandCallBack(DBCommandCallBack* pDBCommandCallback)
{
	CSmartLock lock(m_Lock);
	Assert(pDBCommandCallback != NULL);
	int nID = m_CallbackID++;
	m_ReadyingDBCommandCallback[nID] = pDBCommandCallback;
	return nID;
}

void DBCommandCallBackPool::HeartBeat(float dt, float nowTime)
{
	CSmartLock lock(m_Lock);
	std::map<unsigned int , DBCommandCallBack*>::iterator iter = m_ReadyingDBCommandCallback.begin();
	while (iter != m_ReadyingDBCommandCallback.end())
	{
		DBCommandCallBack* pCallback = iter->second;
		if (pCallback->canExcute())
		{
			pCallback->ExcuteCallBack();
			pCallback->CleanUp();
			m_ReadyingDBCommandCallback.erase(iter++);
			DBCommandCallbackFactoryManager::GetInstancePtr()->GetDBCommandCallbackFactory(pCallback->getType())->DestoryDBCommandCallBack(pCallback);
			continue;
		}
		iter++;
	}
}

bool DBCommandCallBackPool::SignalCommandCallback(unsigned int nCallbackID){
	CSmartLock lock(m_Lock);
	if (m_ReadyingDBCommandCallback.find(nCallbackID) == m_ReadyingDBCommandCallback.end())
	{
		return false;
	}
	m_ReadyingDBCommandCallback[nCallbackID]->Signal();
	return true;
}

DBCommandCallBack* DBCommandCallBackPool::GetCommandCallBack(unsigned int nCallBackID)
{
	CSmartLock lock(m_Lock);
	if (m_ReadyingDBCommandCallback.find(nCallBackID) == m_ReadyingDBCommandCallback.end())
	{
		return NULL;
	}
	return m_ReadyingDBCommandCallback[nCallBackID];
}
