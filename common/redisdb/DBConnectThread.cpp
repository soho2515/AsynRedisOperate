#include "DBConnectThread.h"
#include "Utility.h"
#include "TimeSystem.h"
#include "LogSystem.h"

DBConnectThread::DBConnectThread()
{
	m_ThreadID = -1;
	m_nLastDBHoldTime = 0;
}

DBConnectThread::~DBConnectThread()
{

}

bool DBConnectThread::Instance()
{
	bool bRet = FALSE;
	const char szDBIP[MAX_IP_STR_LEN] = "118.144.76.244";
	unsigned int nDBPort = 6379;
	bRet = m_Interface.ConnectDB(szDBIP, nDBPort);
	if (bRet)
	{
		CacheLog_DEBUG(LOG_DEBUG, "Connect to DB %s:%d succ!", szDBIP, nDBPort);
	}
	else
	{
		CacheLog_DEBUG(LOG_ERROR, "RedisConnector::GetInstancePtr()->ConnectDB error, dbip is %s, dbport is %d", szDBIP, nDBPort);
		return false;
	}

	if (bRet )
	{
		bRet &= m_Operator.Instance();
	}

	m_nLastDBHoldTime = 0;
	return bRet;	
}

void DBConnectThread::OnExit()
{
	m_Interface.DisConnectDB();
	m_Operator.Exit();
	IThread::OnExit();
	return;
}

void DBConnectThread::_Run()
{
	m_ThreadID = GetThreadID();
	INT nSleepTime = 100;
	while (TRUE)
	{
		MySleep(nSleepTime);
		INT nNowTime = CTimeSystem::Instance()->GetMsFromStart();
        try
        {
		    //m_Interface.CheckConnect();
		    INT nComandSize = m_Operator.HeartBeat(nNowTime);
			nSleepTime = GetSleepTime(nComandSize);
		    if (nNowTime-m_nLastDBHoldTime > 1000*200)   //每200秒与DB保持一次连接，以防止DB的8小时（默认）断连接。
		    {
				m_Interface.ExecuteRedisCommand("PING");
			    m_nLastDBHoldTime = nNowTime;
			}
        }
        catch(...)
        {
            CacheLog_DEBUG(LOG_ASSERT, "catch exception in DBConnectThread::_Run()");
        }
	}
}

int DBConnectThread::GetSleepTime( int nLeaveCommandSize )
{
	int nSleepTime = 100;
	if (nLeaveCommandSize <= 5)
	{
		nSleepTime = 100;
	}
	else if (nLeaveCommandSize <= 10)
	{
		nSleepTime = 50;
	}
	else if (nLeaveCommandSize <= 50)
	{
		nSleepTime = 10;
	}
	else if (nLeaveCommandSize <= 100)
	{
		nSleepTime = 5;
	}
	else if (nLeaveCommandSize <= 200)
	{
		nSleepTime = 1;
	}
	else
	{
		nSleepTime = 0;
	}
	return nSleepTime;
}
