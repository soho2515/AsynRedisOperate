#include <algorithm>
#include "AssertDefine.h"
#include "DBCommandCallbackFactoryManager.h"

DBCommandCallbackFactoryManager::DBCommandCallbackFactoryManager()
:m_FactoryPos(0)
{
}

DBCommandCallbackFactoryManager::~DBCommandCallbackFactoryManager()
{

}

bool DBCommandCallbackFactoryManager::Instance()
{
	m_FactoryCache.clear();
	return true;
}

void DBCommandCallbackFactoryManager::Exit()
{
	std::list<DBCommandCallBackFactoryBase*>::iterator iter = m_FactoryCache.begin();
	while (iter != m_FactoryCache.end())
	{
		DBCommandCallBackFactoryBase* pFactory = *iter;
		pFactory->UnInitFactory();
		SAFE_DELETE(pFactory);
	}
	m_FactoryCache.empty();
}

bool DBCommandCallbackFactoryManager::AddDBCommandCallbackFactory(DBCommandCallBackFactoryBase* pFactory)
{
	if (pFactory == NULL)
	{
		return false;
	}

	//同一种类型的Factory只能有一个
	std::list<DBCommandCallBackFactoryBase*>::iterator finditer = std::find(m_FactoryCache.begin(), m_FactoryCache.end(), pFactory);
	Assert(finditer == m_FactoryCache.end());

	if (!pFactory->InitFactory())
	{
		CacheLog_DEBUG(LOG_ERROR, "AddDBCommandCallbackFactory pFactory->InitFactory error");
		return false;
	}

	m_FactoryCache.push_back(pFactory);
	return true;
}

DBCommandCallBackFactoryBase* DBCommandCallbackFactoryManager::GetDBCommandCallbackFactory(const DBCOMMAND_CALLBACK_TYPE type)
{
	DBCommandCallBackFactoryBase* pFactory = NULL;
	std::list<DBCommandCallBackFactoryBase*>::iterator iter = m_FactoryCache.begin();
	while (iter != m_FactoryCache.end())
	{
		pFactory = *iter;
		if (pFactory != NULL && pFactory->GetCallbackType() == type)
		{
			break;
		}
		iter++;
	}
	return pFactory;
}
