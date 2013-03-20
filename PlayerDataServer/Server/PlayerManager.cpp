#include "PlayerManager.h"


Player* PlayerManager::GetPlayerByGuid( Guid_t guid )
{
	__ENTER_FUNCTION__
		std::map<Guid_t, Player*>::iterator it = m_mapPlayers.find(guid);
		if (it != m_mapPlayers.end())
		{
			return it->second;
		}
		return NULL;
	__LEAVE_FUNCTION__}

Player* PlayerManager::GetFreePlayer( Guid_t guid )
{
	__ENTER_FUNCTION__
		Player* pPlayer = (Player *)GetFreeObj();
		if (pPlayer != NULL)
		{
			pPlayer->setGuid(guid);
			m_mapPlayers[guid] = pPlayer;
		}
		return pPlayer;
	__LEAVE_FUNCTION__
}

void PlayerManager::RecyclePlayer( Player* p )
{
	__ENTER_FUNCTION__
		m_mapPlayers.erase(p->getGuid());
		RecycleObj(p);
	__LEAVE_FUNCTION__
}

void PlayerManager::onGameServerClose(ClientConnection* pGameServerConnection)
{
	std::map<Guid_t, Player*>::iterator iter = 	m_mapPlayers.begin();
	while (iter != m_mapPlayers.end())
	{
		Player* pPlayer = iter->second;
		if (pPlayer && pPlayer->GetSelfGameServerConnection() == pGameServerConnection)
		{
			pPlayer->SetSelfGameServerConnection(NULL);
		}
		iter++;
	}
}
