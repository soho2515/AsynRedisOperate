#ifndef __DBCOMMAND_OPERATE_H__
#define __DBCOMMAND_OPERATE_H__

#include <list>
#include "lock.h"
#include "DBCommand.h"

class DBCommandOperate
{
public:
	DBCommandOperate();
	~DBCommandOperate();

	bool Instance();
	int  HeartBeat(INT nNowTime);
	bool Exit();
}; 

#endif
