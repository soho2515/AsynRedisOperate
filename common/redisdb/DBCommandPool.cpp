#include "LogSystem.h"
#include "DBCommandPool.h"
#include "DBCommandCallBackPool.h"

#define MAX_COMMAND_POOL_SIZE    100
#define MAX_COMMAND_POOL_INCREMENT  10

DBCommandPool::DBCommandPool()
{

}

DBCommandPool::~DBCommandPool()
{

}

bool DBCommandPool::Init()
{
	bool bRet = true;
	//bRet = _commandPriority.Init(MAX_COMMAND_POOL_SIZE);
	//bRet &= _commandCommon.Init(MAX_COMMAND_POOL_SIZE);
	//bRet &= _commandCharCache.inithash(1024);
	_commandPriority.clear();
	_commandCommon.clear();
	return bRet;
}

bool DBCommandPool::UnInit()
{
	_commandPriority.clear();
	_commandCommon.clear();
	//_commandCharCache.uninit();
	return true;
}

int DBCommandPool::GetTotalCommandSize()
{
	CSmartLock lock(m_Lock);
	INT nSize = _commandPriority.size() + _commandCommon.size();
	return nSize;
}

bool DBCommandPool::AddPriorityCommand(DBCommand* cmd)
{
	if (cmd == NULL)
	{
		return false;
	}

	cmd->SetPriority(DBCOMMAND_PRIORITY_LEVEL_HIGH);
	{
		CSmartLock lock(m_Lock);
		_commandPriority.push_back(cmd);
	}
	//CacheLog_DEBUG(LOG_DEBUG, "DBCommandPool::AddPriorityCommand, Command Type : %d!", cmd->GetType());
	return true;
}

bool DBCommandPool::AddCommonCommand(DBCommand* cmd)
{
	if (cmd == NULL)
	{
		return false;
	}

	cmd->SetPriority(DBCOMMAND_PRIORITY_LEVEL_MID);
	{
		CSmartLock lock(m_Lock);
		 _commandCommon.push_back(cmd);
	}
	//CacheLog_DEBUG(LOG_DEBUG, "DBCommandPool::AddCommonCommand, Command Type : %d!", cmd->GetType());
	return true;
}

DBCommand* DBCommandPool::GetTopPriorityCommand()
{
	DBCommand* pRet = NULL;
	
	CSmartLock lock(m_Lock);
	if (_commandPriority.size() <= 0)
	{
		return NULL;
	}

	pRet = _commandPriority.front();
	_commandPriority.pop_front();

	return pRet;
}

DBCommand* DBCommandPool::GetTopCommonCommand()
{
	DBCommand* pRet = NULL;

	CSmartLock lock(m_Lock);
	if (_commandCommon.size() <= 0)
	{
		return NULL;
	}

	pRet = _commandCommon.front();
	_commandCommon.pop_front();
	return pRet;
}

void DBCommandPool::ExcureEnd(DBCommand* pCommand)
{
	if (pCommand == NULL)
	{
		return;
	}

	if (pCommand->getCallbackID() > 0)
	{
		DBCommandCallBackPool::GetInstancePtr()->SignalCommandCallback(pCommand->getCallbackID());
	}
	//CacheLog_DEBUG(LOG_DEBUG, "DBCommandPool::ExcureEnd, Command Type : %d!", pCommand->GetType());

	return;
}
