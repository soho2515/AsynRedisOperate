/********************************************************************
	created:	2013/02/18
	created:	18:2:2013   11:52
	filename: 	c:\work\code\Server\common\RedisConnector.h
	file path:	c:\work\code\Server\common
	file base:	RedisConnector
	file ext:	h
	author:		houzhifang
	
	purpose:	
*********************************************************************/
#ifndef __RedisConnector_h__
#define __RedisConnector_h__
#pragma once

#include "Singleton.h"
#include "GlobalDefine.h"
#include "hiredis/hiredis.h"

enum ENUM_REDIS_CONN_STATUS
{
	REDIS_CONN_INIT = 1,		//Redis连接初始状态
	REDIS_CONN_RUNNING,			//Redis连接运行中
	REDIS_CONN_CLOSE,			//Redis连接断开
};

class RedisConnector
{
public:
	RedisConnector():m_pRedisContext(NULL),m_enumConn(REDIS_CONN_INIT),nDBPort(0){
		memset(szDBIp, '\0', MAX_IP_STR_LEN);
	};
	~RedisConnector();
	bool ConnectDB(const char* aszIP, unsigned short awPort);
	bool ReConnectDB();
	bool DisConnectDB();
	redisReply* ExecuteRedisCommand(const char *aszformat, ...);
	//释放Redis应答
	void FreeReply(redisReply* apreply);
	ENUM_REDIS_CONN_STATUS GetConnStatus(void);

private:
	redisContext				*m_pRedisContext;		//Redis连接上下文
	ENUM_REDIS_CONN_STATUS		m_enumConn;				//Redis连接状态
	char szDBIp[MAX_IP_STR_LEN];
	unsigned short nDBPort;
};

#endif // __RedisConnector_h__


