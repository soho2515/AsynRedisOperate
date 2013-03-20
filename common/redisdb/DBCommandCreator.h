/********************************************************************
	created:	2013/02/19
	created:	19:2:2013   15:00
	filename: 	c:\work\code\Server\PlayerDataServer\Server\DBCommandCreator.h
	file path:	c:\work\code\Server\PlayerDataServer\Server
	file base:	DBCommandCreator
	file ext:	h
	author:		houzhifang
	
	purpose:	
*********************************************************************/

#ifndef __DBCommandCreator_h__
#define __DBCommandCreator_h__
#pragma once

#include "redisdb/DBCommand.h"
//#include "DBCommand_Player.h"
#include "redisdb/DBCommandCallBack.h"
//#include "DBCommandCallback_Player.h"

class DBCommandCreator{
public:
	static DBCommand* CreateDBCommand(const DBCOMMAND_TYPE type);
};

class DBCommandCallbackCreator{
public:
	static DBCommandCallBack* CreateDBCommandCallback(const DBCOMMAND_CALLBACK_TYPE type);
};

#endif // __DBCommandCreator_h__
