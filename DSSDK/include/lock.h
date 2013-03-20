#ifndef __LOCK_H__
#define __LOCK_H__

#ifdef __WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include "wtypes.h"
#elif defined(__LINUX)
    #include <pthread.h> 
#endif

class CLock
{
public:
	
	CLock(void);
	~CLock(void);

    void Lock(void);
    void UnLock(void);


private:

#ifdef __WINDOWS
	// Global variable
	CRITICAL_SECTION m_Section; 
#elif defined(__LINUX)
	pthread_mutex_t m_mutex;
	pthread_mutexattr_t attr;
#endif
	//volatile bool m_bNeedUnlock; 

private:
};

class NullLock
{
public:
	NullLock(){}
	~NullLock(){}

	void Lock(){};
	void UnLock(){};

};

template<class LockT>
class SmartLock
{
private:
public:
    SmartLock(LockT& oLock):m_oLock(oLock)
    {
		m_oLock.Lock();	
    }
    ~SmartLock()
    {		
        m_oLock.UnLock();
    }
    void Lock()
    {

		/*if( m_bNeedUnLock )
		{
			return; 
		}
		m_bNeedUnLock = true; */
        m_oLock.Lock();
    }
    void Unlock()
    {
		/*if( !m_bNeedUnLock )
		{
			return; 
		}
		m_bNeedUnLock = false;*/ 
        m_oLock.UnLock();
    }
private:
    LockT& m_oLock;
};

typedef SmartLock<CLock> CSmartLock;

#endif //__LOCK_H__
