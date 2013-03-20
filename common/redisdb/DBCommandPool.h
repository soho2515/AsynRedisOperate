/*
	文件	：DBCommandPool.h
	简介	：DBCommand的命令池，这里保存的是指针，指针指向工厂产生的那些命令，
	          处理线程从本池中取最需要处理的命令，连接线程根据远方来的需要往池中添加命令
	作者	：侯志芳
	创建日期：2010/01/20

	版权所有(C) 2009-2010 竞秀天下（北京）科技有限公司

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DB_COMMAND_POOL_H__
#define __DB_COMMAND_POOL_H__

#include "ServerDefine.h"
#include "DBCommand.h"

class DBCommandPool : public Singleton<DBCommandPool>{
public:
	bool Init();
	bool UnInit();
	bool AddPriorityCommand(DBCommand* cmd);
	bool AddCommonCommand(DBCommand* cmd);
	DBCommand* GetTopPriorityCommand();
	DBCommand* GetTopCommonCommand();
	int  GetTotalCommandSize();

	//对于存储玩家角色数据，这里有一个控制策略，正在存储中的玩家，如果又来了存储需求，是要等待上一个存储完成后才能进行的，以免出现角色存储发生档案覆盖的问题。
	VOID ExcureEnd(DBCommand* pCommand);

private:
	//本来计划使用线程安全的的，后来发现还是自己控制锁比较好。所以在对下面三个对象进行访问和修改时一定要注意线程安全问题。
	std::list<DBCommand*> _commandPriority;
	std::list<DBCommand*> _commandCommon;

	CLock m_Lock;

	DBCommandPool();
	virtual ~DBCommandPool();
	friend class Singleton<DBCommandPool>;
};

#endif
