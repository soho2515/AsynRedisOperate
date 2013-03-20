#include "DBCommand_Personalbase.h"
#include "TimeSystem.h"
#include "PacketFactoryManager.h"
#include "LogSystem.h"
#include "redisdb/DBConnectThread.h"
#include "redisdb/RedisConnector.h"
#include "redisdb/DBThreadPool.h"
#include "redisdb/DBCommandCreator.h"

#define MAX_COMMAND_LEN   (2000)
/*
// *****************************DBCommand_Player_Load**************************************
bool DBCommand_Personal_Load::InitDBCommand(const Guid_t nGuid)
{
	m_nGuid = nGuid;
	return true;
}

bool DBCommand_Personal_Load::Execute()
{
	Assert(m_nGuid != NULL);

	DBConnectThread* pThread = DBThreadPool::GetInstancePtr()->GetDBConnectThread();
	if (pThread == NULL)
	{
		return FALSE;
	}
	RedisConnector* pDBConnector = pThread->GetDBInterface();
	Assert(pDBConnector);
	return true;
}
*/

// *****************************DBCommand_Player_Save**************************************
bool DBCommand_Personal_Save::InitDBCommand(const PersonalBaseInfo &personalinfo)
{
	return true;
}

bool DBCommand_Personal_Save::Execute()
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


	// 提交事务
	redisDBReply = pDBConnector->ExecuteRedisCommand("EXEC");
	if (redisDBReply)
	{
		pDBConnector->FreeReply(redisDBReply);
	}
	
	return true;
}
