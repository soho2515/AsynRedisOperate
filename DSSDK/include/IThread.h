/*
文件	：IThread.h
简介	：
作者	：侯志芳
创建日期：2009/12/23

最后修改日期	修改者		修改内容
---------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------
*/


#ifndef __ITHREAD_H__
#define __ITHREAD_H__

#include "BaseType.h"

#ifdef __WINDOWS
#define THREADPROC DWORD WINAPI
#elif defined (__LINUX)
#define THREADPROC void*
#endif

class IThread
{
public:
	IThread(void);
	virtual ~IThread(void);
	// 开始线程
	void Start(void);
    // 结束线程
    void Stop(void);
    virtual bool IsRunning();
    static int GetThreadCount(){ return s_ThreadCount; };
protected:
	virtual void _Run(void) = 0;
    virtual VOID OnExit();
private:
	static THREADPROC __ThreadProc(void* lParam);
private:
    volatile bool m_bRun;
    static volatile int s_ThreadCount;
public:
    static void __DecreaseCount(); //仅供sdk内部调用
};
#endif
