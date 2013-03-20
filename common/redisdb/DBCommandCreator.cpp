#include "DBCommandCreator.h"
#include "redisdb/DBCommandFactoryManager.h"
#include "redisdb/DBCommandCallbackFactoryManager.h"
#include "redisdb/DBCommandCallBackPool.h"

DBCommand* DBCommandCreator::CreateDBCommand(const DBCOMMAND_TYPE type)
{
	DBCommandFactoryBase* pDBCommandFactory = DBCommandFactoryManager::GetInstancePtr()->GetDBCommandFactory(type);
	if (pDBCommandFactory == NULL || pDBCommandFactory->GetCommandType() != type)
	{
		CacheLog_DEBUG(LOG_DEBUG, "GetDBCommandFactory faild ! type is %d", type);
		return NULL;
	}

	DBCommand* pDBCommand = pDBCommandFactory->CreateDBCommand();
	if (pDBCommand == NULL)
	{
		CacheLog_DEBUG(LOG_DEBUG, "CreateDBCommand faild ! type is %d", type);
	}
	return pDBCommand;
}

/*
DBCommand_Player_Load* DBCommandCreator::CreatePlayerLoadDBCommand(const Guid_t guid)
{
	DBCommand* pDBCommand = DBCommandCreator::CreateDBCommand(DBCOMMAND_TYPE_PLAYER_LOAD);
	DBCommand_Player_Load* pPlayerLoadDBCommand = (DBCommand_Player_Load*)pDBCommand;
	pPlayerLoadDBCommand->InitDBCommand(0);
	return pPlayerLoadDBCommand;
}
*/

//******************************************************************************************************************
DBCommandCallBack* DBCommandCallbackCreator::CreateDBCommandCallback(const DBCOMMAND_CALLBACK_TYPE type)
{
	DBCommandCallBackFactoryBase*  pDBCommandCallBackFactory = DBCommandCallbackFactoryManager::GetInstancePtr()->GetDBCommandCallbackFactory(type);
	if (pDBCommandCallBackFactory == NULL || pDBCommandCallBackFactory->GetCallbackType() != type)
	{
		CacheLog_DEBUG(LOG_DEBUG, "pDBCommandCallBackFactory faild ! type is %d", type);
		return NULL;
	}
	
	DBCommandCallBack* pDBCommandCallback = pDBCommandCallBackFactory->CreateDBCommandCallback();
	if (pDBCommandCallback == NULL)
	{
		CacheLog_DEBUG(LOG_DEBUG, "CreateDBCommandCallback faild ! type is %d", type);
	}

	return pDBCommandCallback;
}

