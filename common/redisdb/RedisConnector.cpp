#include "RedisConnector.h"
#include "AssertDefine.h"

RedisConnector::~RedisConnector(){
	DisConnectDB();
}

bool RedisConnector::ConnectDB(const char* aszIP, unsigned short  awPort)
{
	Assert(m_pRedisContext==NULL);
	strncpy(szDBIp, aszIP, MAX_IP_STR_LEN);
	nDBPort = awPort;

	struct timeval tv;				//1.5分钟
	tv.tv_sec = 1;
	tv.tv_usec = 500000;
	m_pRedisContext = redisConnectWithTimeout(aszIP, awPort, tv);
	if (m_pRedisContext ==NULL || 0 != m_pRedisContext->err)
	{
		return false;
	}
	m_enumConn = REDIS_CONN_RUNNING;

	return true;
}

bool RedisConnector::DisConnectDB()
{
	if (m_pRedisContext != NULL)
	{
		redisFree(m_pRedisContext);
		m_pRedisContext = NULL;
	}
	return true;
}

bool RedisConnector::ReConnectDB()
{
	DisConnectDB();
	if (strlen(szDBIp) > 0 && nDBPort > 0 && ConnectDB(szDBIp, nDBPort))
	{
		return true;
	}
	return false;
}

//执行一条Redis指令
redisReply* RedisConnector::ExecuteRedisCommand(const char *aszformat, ...)
{
	redisReply *lpReply = NULL;
	if (m_pRedisContext == NULL)
	{
		return NULL;
	}
	if (m_enumConn != REDIS_CONN_RUNNING)
	{
		return NULL;
	}

	va_list lap;
	va_start(lap,aszformat);
	lpReply = (redisReply*)redisvCommand(m_pRedisContext,aszformat, lap);

	if (lpReply == NULL)
	{
		switch(m_pRedisContext->err)
		{
		case REDIS_ERR_PROTOCOL:
		case REDIS_ERR_IO:
		case REDIS_ERR_EOF:
		case REDIS_ERR_OTHER:
			//设置连接失败
			m_enumConn = REDIS_CONN_CLOSE;
			CacheLog_DEBUG(LOG_ERROR, "RedisConnector::ExecuteRedisCommand error: %s", aszformat);
			//关闭连接
			DisConnectDB();
			break;
		default:
			break;
		}
		//CacheLog_DEBUG(LOG_ERROR, aszformat, lap);
	}
	va_end(lap);

	return lpReply;
}

void RedisConnector::FreeReply(redisReply* apreply){
	if(NULL != apreply)
	{
		freeReplyObject(apreply);
		apreply = NULL;
	}
}

ENUM_REDIS_CONN_STATUS RedisConnector::GetConnStatus(void)
{
	return m_enumConn;
}

