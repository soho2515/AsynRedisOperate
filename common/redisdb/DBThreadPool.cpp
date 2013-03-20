#include "LogSystem.h"
#include "DBThreadPool.h"
#include "Utility.h"

DBThreadPool::DBThreadPool()
:_db_thread(NULL)
{

}

DBThreadPool::~DBThreadPool()
{

}

bool DBThreadPool::Instance()
{
	bool bRet = true;
	if (_db_thread != NULL)
	{
		return false;
	}

	_db_thread = new DBConnectThread[MAX_DB_CONNECT_THREAD_SIZE];
	if (_db_thread != NULL)
	{
		for (int i = 0; i < MAX_DB_CONNECT_THREAD_SIZE; i++)
		{
			bRet &= _db_thread[i].Instance();
		}
	}

	//if (!bRet)
	//{
	//	for (int i = 0; i < MAX_DB_CONNECT_THREAD_SIZE; i++)
	//	{
	//		_db_thread[i].Exit();
	//	}
	//}

	if (bRet)
	{
		CacheLog_DEBUG(LOG_DEBUG, "DBThreadPool::Instance total thread pool num: %d!", MAX_DB_CONNECT_THREAD_SIZE);
	}

	return bRet;
}

bool DBThreadPool::Begin()
{
	if (_db_thread != NULL)
	{
		for (int i = 0; i < MAX_DB_CONNECT_THREAD_SIZE; i++)
		{
			_db_thread[i].Start();
			CacheLog_DEBUG(LOG_DEBUG, "DBThreadPool::Begin ThreadPool Num: %d begin work!", i);
		}
	}
	return true;
}

void DBThreadPool::Exit()
{
	if (_db_thread != NULL)
	{
		for (int i = 0; i < MAX_DB_CONNECT_THREAD_SIZE; i++)
		{
			_db_thread[i].Stop();
		}
	}	
}

DBConnectThread* DBThreadPool::GetDBConnectThread() const
{
	ULONG nThreadID = GetThreadID();
	for (int i = 0; i < MAX_DB_CONNECT_THREAD_SIZE; i++)
	{
		if (_db_thread[i].GetConnectThreadID() == nThreadID)
		{
			return &_db_thread[i];
		}
	}
	return NULL;
}

int DBThreadPool::GetRunningThreadSize()
{
	int nSize = 0;
	for (int i = 0; i < MAX_DB_CONNECT_THREAD_SIZE; i++)
	{
		if (_db_thread[i].IsRunning())
		{
			nSize++;
		}
	}

	return nSize;
}

