#include "lock.h"

CLock::CLock()
{
	//m_bNeedUnlock = false; 
	// Initialize the critical section one time only.
#ifdef __WINDOWS
    InitializeCriticalSection(&m_Section); 
#elif defined(__LINUX)
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_mutex,&attr);	
#endif
    //ReLock(); 	
}

CLock::~CLock()
{
	UnLock();
#ifdef __WINDOWS
    ::DeleteCriticalSection(&m_Section);
#elif defined(__LINUX)
    pthread_mutex_destroy(&m_mutex);
	pthread_mutexattr_destroy(&attr);
#endif
}

void CLock::UnLock()
{
 /*   m_bNeedUnlock = false;*/
#ifdef __WINDOWS
	LeaveCriticalSection(&m_Section);
#elif defined(__LINUX)
	pthread_mutex_unlock(&m_mutex);	
#endif
}

void CLock::Lock()
{
#ifdef __WINDOWS
	EnterCriticalSection( &m_Section ); 
#elif defined(__LINUX)
	pthread_mutex_lock (&m_mutex);
#endif
    //m_bNeedUnlock = true;
}
