#include "PlayerDataServer.h"
#include "redisdb/DBCommandFactoryManager.h"
#include "redisdb/DBCommandCallbackFactoryManager.h"
#include "DBCommand_Player.h"
#include "DBCommandCallback_Player.h"

bool PlayerDataServer::RegisterAllDBCommand()
{
	bool bRet = true;

	bRet &= DBCommandFactoryManager::GetInstancePtr()->AddDBCommandFactory(new DBCommandFactory<DBCommand_Player_Load>(DBCOMMAND_TYPE_PLAYER_LOAD));
	bRet &= DBCommandFactoryManager::GetInstancePtr()->AddDBCommandFactory(new DBCommandFactory<DBCommand_Player_Save>(DBCOMMAND_TYPE_PLAYER_SAVE));

	return bRet;
}

bool PlayerDataServer::RegisterAllDBCommandCallBack()
{
	bool bRet = true;

	bRet &= DBCommandCallbackFactoryManager::GetInstancePtr()->AddDBCommandCallbackFactory(new DBCommandCallBackFactory<DBCommandCallback_AskPlayerInfo>(DBCOMMAND_CALLBACK_ASKPLAYERINFO));

	return bRet;
}
