#include "IThread.h"

#include "BaseType.h"

#include <time.h>
#include <stdlib.h>

#ifdef __WINDOWS
//#include <windows.h>
#elif defined (__LINUX)
#include <pthread.h>
#endif

volatile int IThread::s_ThreadCount = 0;


IThread::IThread(void)
{
    m_bRun = false;
}

IThread::~IThread(void)
{
}

// 开始线程
void IThread::Start(void)
{
    m_bRun = true;
#ifdef __WINDOWS
    ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(__ThreadProc),this ,0 ,NULL);
#else
    pthread_t uThreadID;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&uThreadID,&attr, __ThreadProc, this);
#endif
}

THREADPROC IThread::__ThreadProc(void* lParam) 
{
    srand( (unsigned)time( NULL ) + (unsigned)IThread::s_ThreadCount );
    IThread* pThread = (IThread *)lParam;
    IThread::s_ThreadCount++;
    pThread->_Run();
    pThread->OnExit();
    IThread::s_ThreadCount--;
    return NULL;
}

void IThread::Stop()
{
    m_bRun = false;
}

VOID IThread::OnExit()
{
	m_bRun = false;
}

bool IThread::IsRunning()
{
    return m_bRun;
}

void IThread::__DecreaseCount()
{
    IThread::s_ThreadCount--;
}

