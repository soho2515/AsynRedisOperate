/*
文件	：TimeSystem.h
简介	：
作者	：侯志芳
创建日期：2009/12/23

最后修改日期	修改者		修改内容
---------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------
*/

#ifndef	_TIME_SYSTEM_H_
#define	_TIME_SYSTEM_H_

#include "BaseType.h"
#include <time.h>
#include "DateTime.h"

class CTimeSystem
{
public:
	CTimeSystem(){}
	~CTimeSystem(){}

public:
	static CTimeSystem* Instance();
	VOID InitialStartUp();
	
public:
	//1970 至今的秒数和毫秒数
	LONGLONG GetBigSecond();
	LONGLONG GetBigMs();

	//相对时间，自程序启动开始：
	//系统启动至今的秒数
	INT GetSecondFromStart();
	//系统启动至今的毫秒数
	INT GetMsFromStart();
private:
	static CTimeSystem*	m_pInstance;	

private:
	DateTime		m_dateTime;
};


#endif
