/*
	文件	：DBCommandFactoryManager.h
	简介	：DB命令工厂的管理类，将通过本类得到真正需要的工厂。
	作者	：侯志芳
	创建日期：2013/02/19

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/
#ifndef __DBCOMMAND_CALLBACK_FACTORY_MANAGER_H__
#define __DBCOMMAND_CALLBACK_FACTORY_MANAGER_H__
#include <list>
#include "BaseType.h"
#include "Singleton.h"
#include "DBCommandCallBack.h"
#include "DBCommandCallbackFactory.h"

class DBCommandCallbackFactoryManager : public Singleton<DBCommandCallbackFactoryManager>
{
public:
	DBCommandCallbackFactoryManager();
	virtual ~DBCommandCallbackFactoryManager();

	bool       Instance();
	void       Exit();
	bool       AddDBCommandCallbackFactory(DBCommandCallBackFactoryBase* pFactory);
	DBCommandCallBackFactoryBase* GetDBCommandCallbackFactory(const DBCOMMAND_CALLBACK_TYPE type);

private:
	//DBCommandFactoryBase* m_FactoryCache[MAX_DBCOMMAND_FACTORY_SIZE];  //替换为下方自己实现的list了，可以自增长大小，自己可以控制。
	std::list<DBCommandCallBackFactoryBase*> m_FactoryCache;
	INT m_FactoryPos;

	friend class Singleton<DBCommandCallbackFactoryManager>;
};

#endif  // __DBCOMMAND_CALLBACK_FACTORY_MANAGER_H__
