/**********************************************************************
	文件:		PlayerMgr.h
	简介	
	作者:		dujiang
	创建日期:	2012/12/6   15:37

	版权所有(C) 2012

	最后修改日期	修改者		修改内容
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/

#ifndef PlayerMgr_h__
#define PlayerMgr_h__

#pragma once

#include "Singleton.h"
#include "Player.h"

//typedef ObjManager<Player, PLAYER_OBJ_ID_BASE> PlayerObjManager;
class PlayerManager : public  ObjManager<Player, PLAYER_OBJ_ID_BASE>, public Singleton<PlayerManager>
{
public:
	Player*		GetPlayerByGuid(Guid_t guid);
	Player*		GetFreePlayer(Guid_t guid);
	void		RecyclePlayer(Player* p);
	void      onGameServerClose(ClientConnection* pGameServerConnection);

private:
	std::map<Guid_t, Player*> m_mapPlayers;
};


#endif // PlayerMgr_h__

