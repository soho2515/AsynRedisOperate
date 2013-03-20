#include "DBCommandCallback_Player.h"
#include "PlayerDataServer.h"

void DBCommandCallback_AskPlayerInfo::Init(const Guid_t nAskGuid, const Guid_t nTargetGUID,  ID_t nClientConnectionID, bool bSendPersonalBaseInfo /*=  false */)
{
	m_nAskGUID = nAskGuid;
	m_nTargetGUID = nTargetGUID;
	m_nClientConnectionID = nClientConnectionID;
	m_bSendPersonalBaseInfo = bSendPersonalBaseInfo;
}

bool DBCommandCallback_AskPlayerInfo::SetCallbackData(const PlayerInfo& playerinfo)
{
	CSmartLock lock(m_datalock);
	m_PlayerInfo = playerinfo;
	return true;
}

void DBCommandCallback_AskPlayerInfo::ExcuteCallBack()
{
	/*
	Player* pPlayer = PlayerManager::GetInstancePtr()->GetFreePlayer(m_nTargetGUID);
	Assert(pPlayer != NULL);

	if (m_nAskGUID == m_nTargetGUID)
	{
		pPlayer->SetSelfGameServerConnection(pClientConnection);
	}
	{
		CSmartLock lock(m_datalock);
		pPlayer->InitWithData(m_PlayerInfo);
	}
	pPlayer->loadAchievementsFromDB();
*/
}
