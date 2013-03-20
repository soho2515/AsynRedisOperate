#include "WorldServerConnector.h"
#include "../../Public/Packets/ServerLoginMsg.h"
#include "AssertDefine.h"


WorldServerConnector::WorldServerConnector(void)
{
__ENTER_FUNCTION__
	
__LEAVE_FUNCTION__
}


WorldServerConnector::~WorldServerConnector(void)
{
__ENTER_FUNCTION__
	
__LEAVE_FUNCTION__
}

void WorldServerConnector::onConnected()
{
	__ENTER_FUNCTION__
		// !! TO DO [11/29/2012 dujiang] 从配置文件中读取
		ServerAddr listenAddr;
		strncpy(listenAddr.IP, "127.0.0.1", MAX_IP_STR_LEN);
		listenAddr.Port = 9000;
	
		ServerLoginMsg  msg;
		msg.setListenAddr(listenAddr);
		msg.setServerType(SERVER_TYPE_PLAYER_DATA);
	
		SendMsg(&msg);
	__LEAVE_FUNCTION__
}