#include "DateTime.h"
#include <stdio.h>


DateTime::DateTime()
{
#ifdef __LINUX
	struct timezone tz;
	gettimeofday(&m_tv, &tz);
#else
	time(&m_time);
	SYSTEMTIME sTime;
	GetSystemTime(&sTime);
	m_milliSecond = sTime.wMilliseconds;
#endif
}

DateTime::DateTime( int second, int milliSecond )
{
#ifdef __LINUX
	m_tv.tv_sec = second;
	m_tv.tv_usec = milliSecond * 1000;
#else
	m_time = second;
	m_milliSecond = milliSecond;
#endif
}

INT DateTime::GetYear()
{
#ifdef __LINUX
	struct tm stm;
	localtime_r(&m_tv.tv_sec, &stm);
	struct tm * ptm = &stm;
#else
	struct tm *ptm = localtime(&m_time);
#endif
	return 1900 + ptm->tm_year;
}

INT DateTime::GetMonth()
{
#ifdef __LINUX
	struct tm stm;
	localtime_r(&m_tv.tv_sec, &stm);
	struct tm * ptm = &stm;
#else
	struct tm *ptm = localtime(&m_time);
#endif
	return ptm->tm_mon+1;
}

INT DateTime::GetDay()
{
#ifdef __LINUX
	struct tm stm;
	localtime_r(&m_tv.tv_sec, &stm);
	struct tm * ptm = &stm;
#else
	struct tm *ptm = localtime(&m_time);
#endif
	return ptm->tm_mday;
}

INT DateTime::GetWeekDay()
{
#ifdef __LINUX
	struct tm stm;
	localtime_r(&m_tv.tv_sec, &stm);
	struct tm * ptm = &stm;
#else
	struct tm *ptm = localtime(&m_time);
#endif
	return ptm->tm_wday;
}

INT DateTime::GetHour()
{
#ifdef __LINUX
	struct tm stm;
	localtime_r(&m_tv.tv_sec, &stm);
	struct tm * ptm = &stm;
#else
	struct tm *ptm = localtime(&m_time);
#endif
	return ptm->tm_hour;
}

INT DateTime::GetMinute()
{
#ifdef __LINUX
	struct tm stm;
	localtime_r(&m_tv.tv_sec, &stm);
	struct tm * ptm = &stm;
#else
	struct tm *ptm = localtime(&m_time);
#endif
	return ptm->tm_min;

}

INT DateTime::GetSecond()
{
#ifdef __LINUX
	struct tm stm;
	localtime_r(&m_tv.tv_sec, &stm);
	struct tm * ptm = &stm;
#else
	struct tm *ptm = localtime(&m_time);
#endif
	return ptm->tm_sec;

}

INT DateTime::GetMilliSecond()
{
#ifdef __LINUX
	return m_tv.tv_usec/1000;
#else
	return m_milliSecond;
#endif
}

LONGLONG DateTime::GetBigSecond()
{
#ifdef __LINUX
	return m_tv.tv_sec;
#else
	return m_time;
#endif
}

LONGLONG DateTime::GetBigMs()
{
#ifdef __LINUX
	return (LONGLONG)m_tv.tv_sec*1000+ (LONGLONG)m_tv.tv_usec/1000;
#else
	return (LONGLONG)m_time*1000 + (LONGLONG)m_milliSecond;
#endif
}
