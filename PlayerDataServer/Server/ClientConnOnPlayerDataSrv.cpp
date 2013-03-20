#include "ClientConnOnPlayerDataSrv.h"
#include "PersonalBaseManager.h"
#include "PlayerManager.h"

ClientConnOnPlayerDataSrv::ClientConnOnPlayerDataSrv(void)
{
}

ClientConnOnPlayerDataSrv::~ClientConnOnPlayerDataSrv(void)
{
}

void ClientConnOnPlayerDataSrv::onClose()
{
	// !! TO DO [12/10/2012 dujiang] 将来还有其它类型注册的玩家和一个连接的也要在这加代码
	PersonalBaseManager::GetInstancePtr()->removeVisitorFromConneciton(this);
	PlayerManager::GetInstancePtr()->onGameServerClose(this);
}
