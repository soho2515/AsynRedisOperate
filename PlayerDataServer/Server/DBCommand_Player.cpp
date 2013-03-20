#include "DBCommand_Player.h"
#include "TimeSystem.h"
#include "LogSystem.h"
#include "redisdb/DBConnectThread.h"
#include "redisdb/RedisConnector.h"
#include "redisdb/DBThreadPool.h"
#include "redisdb/DBCommandCreator.h"
#include "redisdb/DBCommandCallBackPool.h"
#include "DBCommandCallback_Player.h"

#define MAX_COMMAND_LEN   (2000)

// *****************************DBCommand_Player_Load**************************************
bool DBCommand_Player_Load::InitDBCommand(const Guid_t nGuid)
{
	m_nGuid = nGuid;
	return true;
}

bool DBCommand_Player_Load::Execute()
{
	Assert(m_nGuid != NULL);

	DBConnectThread* pThread = DBThreadPool::GetInstancePtr()->GetDBConnectThread();
	if (pThread == NULL)
	{
		return FALSE;
	}

	RedisConnector* pDBConnector = pThread->GetDBInterface();
	Assert(pDBConnector);

	/*
	PlayerInfo info;
	info.clear();
	info.m_nPlayerGuid = m_nGuid;

	char szCommand[MAX_COMMAND_LEN+1] = {0};
	_snprintf(szCommand, MAX_COMMAND_LEN, "MGET %d_playername %d_exp %d_nextexp %d_money %d_titleid %d_clanguid %d_clansymid %d_iconid %d_clanname",        \
		m_nGuid, m_nGuid, m_nGuid, m_nGuid, m_nGuid, m_nGuid, m_nGuid, m_nGuid, m_nGuid);

	redisReply* redisDBReply = NULL;
	redisDBReply = pDBConnector->ExecuteRedisCommand(szCommand);
	if (redisDBReply)
	{
		Assert(redisDBReply->elements == 9);
		if (redisDBReply->element[0]->type != REDIS_REPLY_NIL)
		{
			strncpy(info.m_szName, redisDBReply->element[0]->str, PLAYER_NAME_WITH_CLOSING);
		}
		if (redisDBReply->element[1]->type != REDIS_REPLY_NIL)
		{
			info.m_nExp = atoi(redisDBReply->element[1]->str);
		}
		if (redisDBReply->element[2]->type != REDIS_REPLY_NIL){
			info.m_nNextLevelNeedExp = atoi(redisDBReply->element[2]->str);
		}
		if (redisDBReply->element[3]->type != REDIS_REPLY_NIL){
			info.m_nMoneyValue = atoi(redisDBReply->element[3]->str);
		}
		if (redisDBReply->element[4]->type != REDIS_REPLY_NIL){
			info.m_nTitleID = atoi(redisDBReply->element[4]->str);
		}
		if (redisDBReply->element[5]->type != REDIS_REPLY_NIL){
			info.m_nClanGuid = atoi(redisDBReply->element[5]->str);
		}
		if (redisDBReply->element[6]->type != REDIS_REPLY_NIL){
			info.m_nClanSymbolID = atoi(redisDBReply->element[6]->str);
		}
		if (redisDBReply->element[7]->type != REDIS_REPLY_NIL){
			info.m_nIconID = atoi(redisDBReply->element[7]->str);
		}
		if (redisDBReply->element[8]->type != REDIS_REPLY_NIL){
			strncpy(info.m_szClanName, redisDBReply->element[8]->str, CLAN_NAME_WITH_CLOSING);
		}

		pDBConnector->FreeReply(redisDBReply);
		CacheLog_DEBUG(LOG_DEBUG, "ExecuteRedisCommand OK. %s", szCommand);
	}
	else
	{
		CacheLog_DEBUG(LOG_ERROR, "ExecuteRedisCommand fail. %s", szCommand);
		return false;
	}

	// 赋值
	Assert(this->getCallbackID() > 0);
	DBCommandCallback_AskPlayerInfo* pAskPlayerInfoCallBack = (DBCommandCallback_AskPlayerInfo*)DBCommandCallBackPool::GetInstancePtr()->GetCommandCallBack(this->getCallbackID());
	if (pAskPlayerInfoCallBack != NULL)
	{
		pAskPlayerInfoCallBack->SetCallbackData(info);
	}
	else
	{
		CacheLog_DEBUG(LOG_ERROR, "cannot find DBCommandCallback_AskPlayerInfo for id: %d", this->getCallbackID());
	}
	*/
	
	return true;
}

// *****************************DBCommand_Player_Save**************************************
bool DBCommand_Player_Save::InitDBCommand(const PlayerInfo &playerinfo, const int nChangeFlag)
{
	return true;
}

bool DBCommand_Player_Save::Execute()
{
	DBConnectThread* pThread = DBThreadPool::GetInstancePtr()->GetDBConnectThread();
	if (pThread == NULL)
	{
		return FALSE;
	}

	RedisConnector* pDBConnector = pThread->GetDBInterface();
	Assert(pDBConnector);

	redisReply* redisDBReply = NULL;
	// 事务开始
	redisDBReply = pDBConnector->ExecuteRedisCommand("MULTI");
	if (redisDBReply)
	{
		pDBConnector->FreeReply(redisDBReply);
	}

	/*
	// 保存普通信息
	char szCommand[MAX_COMMAND_LEN+1] = {0};
	int nGuid = m_PlayerInfo.m_nPlayerGuid;
	_snprintf(szCommand, MAX_COMMAND_LEN, "MSET %d_playername %s %d_exp %d %d_nextexp %d %d_money %d %d_titleid %d %d_clanguid %d %d_clansymid %d %d_iconid %d %d_clanname %s",        \
		nGuid, m_PlayerInfo.m_szName, nGuid, m_PlayerInfo.m_nExp, nGuid, m_PlayerInfo.m_nNextLevelNeedExp, nGuid, m_PlayerInfo.m_nMoneyValue,                  \
		nGuid, m_PlayerInfo.m_nUseTitleOfAchievementId, nGuid, m_PlayerInfo.m_nClanGuid, nGuid, m_PlayerInfo.m_nClanSymbolID,                                                \
		nGuid, m_PlayerInfo.m_nIconID, nGuid, m_PlayerInfo.m_szClanName);

	redisDBReply = NULL;
	redisDBReply = pDBConnector->ExecuteRedisCommand(szCommand);
	if (redisDBReply)
	{
		pDBConnector->FreeReply(redisDBReply);
		CacheLog_DEBUG(LOG_DEBUG, "ExecuteRedisCommand OK. %s", szCommand);
	}
	else
	{
		CacheLog_DEBUG(LOG_ERROR, "ExecuteRedisCommand fail. %s", szCommand);
	}
	*/

	// 提交事务
	redisDBReply = pDBConnector->ExecuteRedisCommand("EXEC");
	if (redisDBReply)
	{
		pDBConnector->FreeReply(redisDBReply);
	}
	
	return true;
}
