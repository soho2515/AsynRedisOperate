/*
	文件	：ServerBase.h
	简介	：服务器程序的基类 app
	作者	：cuiyinjie
	创建日期：2010/01/06

	版权所有(C) 2009-2010 竞秀天下（北京）科技有限公司

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __SERVERBASE_H__
#define __SERVERBASE_H__

#include "BaseType.h"
#include "Utility.h"

#include <time.h>
#include <stdlib.h>

// 实现网络环境的初始化与清除，服务器初始化与运行时机等
class CServerBase
{
public:
    CServerBase(){ m_nSleepTimeMs = 1000;m_nLastPerSecondsProcessTime=0; m_bNetWorkStarted = FALSE;};
    virtual ~CServerBase(){ CleanUp(); };
    bool Init();
    VOID Start();
    VOID Stop();
    VOID SetSleepTimeMs(INT nMilliseconds);;
    VOID CleanUp();

protected:
    virtual VOID DoStart() = 0;
	virtual VOID DoTick() = 0;
	VOID DoLogFlush();
    virtual bool DoInitGameData() = 0;
    virtual const CHAR* ToString(){ return "ServerBase"; };
    virtual VOID OnExit(){}; // 退出的处理
	static	VOID sdkLogCallBack(const CHAR* szLogText);

private:
    bool	InitNetWork(); 
	void	ProcessPerSeconds();
	void	StopCheck();

private:
    volatile bool m_bRun;
    INT m_nSleepTimeMs;
	bool m_bNetWorkStarted;
	INT m_nLastPerSecondsProcessTime;
};

#endif // __SERVERBASE_H__
