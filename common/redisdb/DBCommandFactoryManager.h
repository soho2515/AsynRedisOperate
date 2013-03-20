/*
	文件	：DBCommandFactoryManager.h
	简介	：DB命令工厂的管理类，将通过本类得到真正需要的工厂。
	作者	：侯志芳
	创建日期：2010/01/04

	版权所有(C) 2009-2010 竞秀天下（北京）科技有限公司

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/
#ifndef __DBCOMMAND_FACTORY_MANAGER_H__
#define __DBCOMMAND_FACTORY_MANAGER_H__
#include <list>
#include "BaseType.h"
#include "Singleton.h"
#include "DBCommand.h"
#include "DBCommandFactory.h"

class DBCommandFactoryManager : public Singleton<DBCommandFactoryManager>
{
public:
	DBCommandFactoryManager();
	virtual ~DBCommandFactoryManager();

	bool       Instance();
	void       Exit();
	bool       AddDBCommandFactory(DBCommandFactoryBase* pFactory);
	DBCommandFactoryBase* GetDBCommandFactory(const DBCOMMAND_TYPE type);

private:
	//DBCommandFactoryBase* m_FactoryCache[MAX_DBCOMMAND_FACTORY_SIZE];  //替换为下方自己实现的list了，可以自增长大小，自己可以控制。
	std::list<DBCommandFactoryBase*> m_FactoryCache;
	INT m_FactoryPos;

	friend class Singleton<DBCommandFactoryManager>;
};

#endif
