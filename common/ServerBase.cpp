#include "ServerBase.h"
#include "TimeSystem.h"
#include "LogSystem.h"
#include "AssertDefine.h"
#include "Utility.h"
#include <fstream>

bool CServerBase::InitNetWork()
{
__ENTER_FUNCTION__
	#ifdef __WINDOWS
		SHORT sVersionRequested;
		WSADATA wsaData;
		int err;
	
	
		// !! TO DO [11/14/2012 dujiang] 现在就装的是64位的win7, 调试一下
		//64下会有问题， 但windows下不跑64位server吧
		sVersionRequested = MAKEWORD( 2, 2 );
	
		err = WSAStartup( sVersionRequested, &wsaData );
		if ( err != 0 ) 
		{
			return FALSE;
		}
		else
		{
			m_bNetWorkStarted = TRUE;
		}
	#endif
		return TRUE; 
__LEAVE_FUNCTION__
}

VOID CServerBase::CleanUp()
{
__ENTER_FUNCTION__
	#ifdef __WINDOWS
		if (m_bNetWorkStarted)
		{
			WSACleanup();
			m_bNetWorkStarted = FALSE;
		}
	#endif
__LEAVE_FUNCTION__
}

bool CServerBase::Init()
{
	__ENTER_FUNCTION__
	try
		{
			srand( (unsigned)time( NULL ) );
			// InitSigHandler
			// InitLogSystem
			CLogSystem::Instance()->Initialize();
	
			bool bRet = TRUE;
			bRet = InitNetWork();
			if (!bRet)
			{
				return bRet;
			}
	
			bRet = DoInitGameData();
			AssertEx(bRet, "DoInitGameData() failed");
			if (!bRet )
			{
				return bRet;
			}
	
			return bRet;
		}
		catch(...)
		{
			CacheLog_ERROR(LOG_ERROR, "CServerBase::Init() return FALSE");
			CLogSystem::Instance()->Flush();
			return FALSE;
		}
__LEAVE_FUNCTION__
}

VOID CServerBase::Start()
{
	__ENTER_FUNCTION__
		DoStart();
		m_bRun = TRUE;
		while (m_bRun)
		{
			try{
				DoTick();
				ProcessPerSeconds();
				MySleep(m_nSleepTimeMs);
			}
			catch (...){
				CacheLog_ERROR(LOG_ASSERT, "CServerBase::Start() catch exception!!");
			}
		}
		OnExit();
	__LEAVE_FUNCTION__
}

void CServerBase::ProcessPerSeconds()
{
	__ENTER_FUNCTION__
	INT nCurrentTime = CTimeSystem::Instance()->GetSecondFromStart();
		INT nDeltaTime = nCurrentTime - m_nLastPerSecondsProcessTime;
		if (nDeltaTime < 1)
			return;
		else
			m_nLastPerSecondsProcessTime = nCurrentTime;	
		
		// 检测服务的时间累计
		static unsigned int _Time = 0;		
		++_Time;
		//CacheLog_DEBUG(LOG_DEBUG, "_time:%d", _Time);
	
		// !! TO DO [12/17/2012 dujiang] 要从配置文件中读取
		if ((_Time % 5) == 0){
			DoLogFlush();
		}
	
		if ((_Time % 5) == 0){
			StopCheck();
		}
__LEAVE_FUNCTION__
}

VOID CServerBase::DoLogFlush()
{
	__ENTER_FUNCTION__
		CLogSystem::Instance()->Flush();
		m_nLastPerSecondsProcessTime = CTimeSystem::Instance()->GetSecondFromStart();
	__LEAVE_FUNCTION__
}

VOID CServerBase::Stop()
{
__ENTER_FUNCTION__
		m_bRun = FALSE;
__LEAVE_FUNCTION__
}

VOID CServerBase::SetSleepTimeMs( INT nMilliseconds )
{
__ENTER_FUNCTION__
		m_nSleepTimeMs = nMilliseconds;
__LEAVE_FUNCTION__
}

VOID CServerBase::sdkLogCallBack( const CHAR* szLogText )
{
	__ENTER_FUNCTION__
		if (szLogText != NULL){
			CacheLog_DEBUG(LOG_DSSDK, szLogText);
		}
	__LEAVE_FUNCTION__
}

void CServerBase::StopCheck()
{
	__ENTER_FUNCTION__
		const char filename[] = "exit.txt";
	
		std::fstream file;
		file.open(filename, std::ios::in);
		// 文件存在
		if (file)
		{
			CacheLog_DEBUG(LOG_DEBUG, "find exit.txt file, to exit program\n");
			Stop();
		}
	__LEAVE_FUNCTION__
}
