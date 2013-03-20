/*
	文件	：DBThreadPool.h
	简介	：负责维持所有DB连接的处理线程
	作者	：侯志芳
	创建日期：2010/01/20

	版权所有(C) 2009-2010 竞秀天下（北京）科技有限公司

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DB_THREAD_POOL_H__
#define __DB_THREAD_POOL_H__

#include "Singleton.h"
#include "DBConnectThread.h"

#define MAX_DB_CONNECT_THREAD_SIZE  1

class DBThreadPool : public Singleton<DBThreadPool>
{
public:
	DBThreadPool();
	virtual ~DBThreadPool();

	bool Instance();
	bool Begin();
	void Exit();

	int  GetRunningThreadSize();

	DBConnectThread* GetDBConnectThread() const ;

private:
	DBConnectThread* _db_thread;

	friend class Singleton<DBThreadPool>;
};

#endif
