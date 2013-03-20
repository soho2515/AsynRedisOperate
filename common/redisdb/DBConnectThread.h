/*
	文件	：DBConnectThread.h
	简介	：db处理线程，每个线程保持一个DB得长连接，每帧从命令池中取最紧急需要处理的命令进行处理
	作者	：侯志芳
	创建日期：2010/01/20

	版权所有(C) 2009-2010 竞秀天下（北京）科技有限公司

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DB_CONNECT_THREAD_H__
#define __DB_CONNECT_THREAD_H__
#include "IThread.h"
#include "RedisConnector.h"
#include "DBCommandOperate.h"

class DBConnectThread : public IThread
{
public:
	DBConnectThread();
	virtual ~DBConnectThread();

	bool Instance();
	virtual void OnExit();

	RedisConnector* GetDBInterface() {
		return &m_Interface;
	}
	const ULONG GetConnectThreadID() const {
		return m_ThreadID;
	}

protected:
	virtual void _Run(void);

	// 用来根据剩余命令的个数得到心跳频率，从而动态调节DB压力
	int  GetSleepTime(int nLeaveCommandSize);

private:
	RedisConnector m_Interface;
	DBCommandOperate m_Operator;
	ULONG m_ThreadID;
	int   m_nLastDBHoldTime;
};

#endif
