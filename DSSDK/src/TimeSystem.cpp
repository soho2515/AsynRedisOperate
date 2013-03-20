
#include "TimeSystem.h"
#include <stdio.h>

#if defined(__WINDOWS)
# include <sys/timeb.h>
#else
# include <sys/time.h>
#endif

CTimeSystem* CTimeSystem::m_pInstance = NULL;

CTimeSystem* CTimeSystem::Instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CTimeSystem();
		m_pInstance->InitialStartUp();
	}
	return m_pInstance;
}

VOID CTimeSystem::InitialStartUp()
{
	m_dateTime = DateTime();
}

//
//VOID CTimeSystem::UpdateTime()
//{
//#ifdef __WINDOWS
//
//// 	struct __timeb64 timebuffer;
//// 	_ftime64_s( &timebuffer );
//// 
//// 	m_llSecond = timebuffer.time;
//// 	m_usMilliSecond = timebuffer.millitm;
//// 	m_llBigMs = m_llSecond * 1000 + m_usMilliSecond;
//		
//// 	LARGE_INTEGER start, fre;
//// 	QueryPerformanceCounter(&start);
//// 	QueryPerformanceFrequency(&fre);
//// 	SDK_Log("%d\n",start.QuadPart*1000/fre.QuadPart);
//	
//#else
//
//#endif	
//
//}

LONGLONG CTimeSystem::GetBigSecond()
{
	return DateTime().GetBigSecond();
}

LONGLONG CTimeSystem::GetBigMs()
{
	return DateTime().GetBigMs();
}

//系统启动至今的秒数
INT CTimeSystem::GetSecondFromStart()
{
	return DateTime().GetBigSecond() - m_dateTime.GetBigSecond();
}
//系统启动至今的毫秒数
INT CTimeSystem::GetMsFromStart()
{
	return DateTime().GetBigMs() -m_dateTime.GetBigMs();
}
