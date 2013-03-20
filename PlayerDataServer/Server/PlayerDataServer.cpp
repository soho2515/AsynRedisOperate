#include "PlayerDataServer.h"
#include "ServerDefine.h"
#include "redisdb/DBThreadPool.h"
#include "redisdb/DBCommandCallBackPool.h"
#include "redisdb/DBCommandFactoryManager.h"
#include "redisdb/DBCommandCallbackFactoryManager.h"

PlayerDataServer::PlayerDataServer(void)
{
	__ENTER_FUNCTION__

		__LEAVE_FUNCTION__
}

PlayerDataServer::~PlayerDataServer(void)
{
	__ENTER_FUNCTION__

		__LEAVE_FUNCTION__
}

VOID PlayerDataServer::DoStart()
{
	__ENTER_FUNCTION__

	if (!DBThreadPool::GetInstancePtr()->Begin())
	{
		CacheLog_DEBUG(LOG_ERROR, "DBThread begin error. ");
		Assert(false);
	}
	__LEAVE_FUNCTION__
}

VOID PlayerDataServer::DoTick()
{
	__ENTER_FUNCTION__
		static INT start(0);

	try
	{
		INT now = CTimeSystem::Instance()->GetMsFromStart();
		INT deltaTimeMs = now - start;
		if ( 0 == start )
		{
			deltaTimeMs = 0;
		}
		start = now;

		DBCommandCallBackPool::GetInstancePtr()->HeartBeat(deltaTimeMs, now);

		MySleep(0);
	}
	catch(...)
	{
		DiskLog_ERROR(LOG_ASSERT,"ERROR!!! catch CWorldServer::DoTick exception!!");
	}
	__LEAVE_FUNCTION__
}

bool PlayerDataServer::DoInitGameData()
{
	__ENTER_FUNCTION__
		// log 
		CLogSystem::Instance()->SetConsoleLogLevel(LOG_LEVEL_DEBUG);
	CLogSystem::Instance()->SetFileLogLevel(LOG_LEVEL_DEBUG);

	// !! TO DO [11/16/2012 dujiang] 需要换成playerdataserver的config
	const CHAR* szLogPath = "./";
	CHAR szLogFileName[MAX_PATH] = {0};

	bool isOK = TRUE;
	_snprintf(szLogFileName, MAX_PATH, "playerDataServer_debug");
	isOK &= CLogSystem::Instance()->InitLogFile(LOG_DEBUG, szLogFileName, szLogPath);
	_snprintf(szLogFileName, MAX_PATH, "playerDataServer_assert");
	isOK &= CLogSystem::Instance()->InitLogFile(LOG_ASSERT, szLogFileName, szLogPath);
	_snprintf(szLogFileName, MAX_PATH, "playerDataServer_send");
	isOK &= CLogSystem::Instance()->InitLogFile(LOG_SEND, szLogFileName, szLogPath);
	_snprintf(szLogFileName, MAX_PATH, "playerDataServer_s8sdk");
	isOK &= CLogSystem::Instance()->InitLogFile(LOG_DSSDK, szLogFileName, szLogPath);
	if ( !isOK )
	{
		return false;
	}

	SetSdkLogFunction(&sdkLogCallBack);

	bool bRet = DBCommandFactoryManager::GetInstancePtr()->Instance();
	if (!bRet)
	{
		CacheLog_DEBUG(LOG_ERROR, "DBCommandFactoryManager::GetInstancePtr()->Instance error!");
		return false;
	}
	bRet = DBCommandCallbackFactoryManager::GetInstancePtr()->Instance();
	if (!bRet)
	{
		CacheLog_DEBUG(LOG_ERROR, "DBCommandCallbackFactoryManager::GetInstancePtr()->Instance error!");
		return false;
	}
	bRet = RegisterAllDBCommand();
	if (!bRet)
	{
		CacheLog_DEBUG(LOG_ERROR, "RegisterAllDBCommand error!");
		return false;
	}
	bRet = RegisterAllDBCommandCallBack();
	if (!bRet)
	{
		CacheLog_DEBUG(LOG_ERROR, "RegisterAllDBCommandCallBack error!");
		return false;
	}

	//初始化DB连接的线程池
	bRet = DBThreadPool::GetInstancePtr()->Instance();
	if (!bRet)
	{
		CacheLog_DEBUG(LOG_ERROR, "DBThreadPool::GetInstancePtr()->Instance() Error!");
		return bRet;
	}

	return true;
	__LEAVE_FUNCTION__

}

VOID PlayerDataServer::OnExit()
{
	__ENTER_FUNCTION__
		CacheLog_DEBUG(LOG_DEBUG, "WorldServer::OnExit(): begin!");

	DBThreadPool::GetInstancePtr()->Exit();
	DBCommandFactoryManager::GetInstancePtr()->Exit();
	DBCommandCallbackFactoryManager::GetInstancePtr()->Exit();

	CacheLog_DEBUG(LOG_DEBUG, "WorldServer::OnExit(): end!");
	CLogSystem::Instance()->Flush();
	__LEAVE_FUNCTION__
}
