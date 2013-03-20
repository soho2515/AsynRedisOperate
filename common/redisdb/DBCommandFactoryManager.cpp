#include <algorithm>
#include "AssertDefine.h"
#include "DBCommandFactoryManager.h"

DBCommandFactoryManager::DBCommandFactoryManager()
:m_FactoryPos(0)
{
}

DBCommandFactoryManager::~DBCommandFactoryManager()
{

}

bool DBCommandFactoryManager::Instance()
{
	m_FactoryCache.clear();
	return true;
}

void DBCommandFactoryManager::Exit()
{
	std::list<DBCommandFactoryBase*>::iterator iter = m_FactoryCache.begin();
	while (iter != m_FactoryCache.end())
	{
		DBCommandFactoryBase* pFactory = *iter;
		pFactory->UnInitFactory();
		SAFE_DELETE(pFactory);
	}
	m_FactoryCache.empty();
}

bool DBCommandFactoryManager::AddDBCommandFactory(DBCommandFactoryBase* pFactory)
{
	if (pFactory == NULL)
	{
		return false;
	}

	//同一种类型的Factory只能有一个
	std::list<DBCommandFactoryBase*>::iterator finditer = std::find(m_FactoryCache.begin(), m_FactoryCache.end(), pFactory);
	Assert(finditer == m_FactoryCache.end());

	if (!pFactory->InitFactory())
	{
		CacheLog_DEBUG(LOG_ERROR, "DBCommandFactoryManager::AddDBCommandFactory pFactory->InitFactory error.");
		return false;
	}

	m_FactoryCache.push_back(pFactory);
	return true;
}

DBCommandFactoryBase* DBCommandFactoryManager::GetDBCommandFactory(const DBCOMMAND_TYPE type)
{
	DBCommandFactoryBase* pFactory = NULL;
	std::list<DBCommandFactoryBase*>::iterator iter = m_FactoryCache.begin();
	while (iter != m_FactoryCache.end())
	{
		pFactory = *iter;
		if (pFactory != NULL && pFactory->GetCommandType() == type)
		{
			break;
		}
		iter++;
	}
	return pFactory;
}
