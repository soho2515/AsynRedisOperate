#include "DBCommandOperate.h"
#include "DBCommandPool.h"
#include "DBCommandFactoryManager.h"
#include "LogSystem.h"

DBCommandOperate::DBCommandOperate()
{
}

DBCommandOperate::~DBCommandOperate()
{

}

bool DBCommandOperate::Instance()
{
	return true;
}

int DBCommandOperate::HeartBeat(INT nNowTime)
{
	DBCommand* pCmd = DBCommandPool::GetInstancePtr()->GetTopPriorityCommand();
	if (pCmd == NULL)
	{
		pCmd = DBCommandPool::GetInstancePtr()->GetTopCommonCommand();
	}

	if (pCmd != NULL)
	{
		bool bRet = pCmd->Execute();
		if (bRet)
		{
			DBCommandPool::GetInstancePtr()->ExcureEnd(pCmd);  //这个函数，处理角色数据的重复存储问题，防止档案覆盖，目前只有角色存储会用到。
			DBCommandFactoryManager::GetInstancePtr()->GetDBCommandFactory(pCmd->GetType())->DestoryDBCommand(pCmd);
		}
		else
		{
			CacheLog_DEBUG(LOG_ERROR, "DBCommandPool Excure Fial");
		}
	}

	return DBCommandPool::GetInstancePtr()->GetTotalCommandSize();
}

bool DBCommandOperate::Exit()
{
	return true;
}
