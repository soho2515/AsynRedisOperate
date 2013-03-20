#include "Player.h"
#include "MsgID.h"
#include "TableSystem.h"
#include "PublicTableDefine.h"
#include "AchievementsConfig.h"
#include "AllPacketInclude.h"
#include "PersonalBaseManager.h"
#include "DBCommand_Player.h"
#include "redisdb/DBCommandCreator.h"
#include "redisdb/DBCommandPool.h"

Player::Player(void)
	: m_mailMgr(this)
{
	__ENTER_FUNCTION__
	//	m_nGuid = invalid_guid;
	CleanUp();
	__LEAVE_FUNCTION__
}

Player::~Player(void)
{__ENTER_FUNCTION__

__LEAVE_FUNCTION__

}

void Player::CleanUp()
{
	Obj::CleanUp();
	m_nGameState = GAME_STATE_INVALID;
	m_playerInfo.clear();
	m_bOnline = false;
	m_nPlayerInfoChangeFlag = 0;
	m_pGameServerConnection = NULL;
}

bool Player::InitWithData(const PlayerInfo& playerInfo)
{
	m_playerInfo = playerInfo;
	return true;
}

void Player::SetSelfGameServerConnection(ClientConnection* pConnection)
{
	m_pGameServerConnection = pConnection;
}

void Player::SendMsgToSelf(Packet* pPacket)
{
	if (m_pGameServerConnection != NULL)
	{
		m_pGameServerConnection->SendMsg(pPacket);
	}
}

void Player::SendPlayerInfo(ClientConnection* pClientConnection, Guid_t nVisitorGuid)
{
	SCSendPlayerInfo msgPlayerInfo;
	msgPlayerInfo.visitorGuid(nVisitorGuid);
	msgPlayerInfo.targetGuid(this->getGuid());
	msgPlayerInfo.playerInfo(this->playerInfo());
	pClientConnection->SendMsg(&msgPlayerInfo);
}

//bool Player::canDo(PacketId_t nMsgID) const
//{
//	__ENTER_FUNCTION__
//		bool bCanDo = false;
//	switch (m_nGameState)
//	{
//		// 等待登陆（过了login server）
//	case GAME_STATE_WAIT_LOGIN: 
//		{
//
//		} break;
//		// 登陆通过了，等待玩家选择去哪
//	case GAME_STATE_LOGIN_PASSED: 
//		{
//
//		} break;
//		// 玩家在自己的基地中
//	case GAME_STATE_IN_SELF_BASE:	
//		{
//			if (nMsgID == MSGID_CS_CREATEBUILDING ||
//				nMsgID == MSGID_CS_BUILDING_OPERATION ||
//				nMsgID == MSGID_CS_SOLDIERTRAININGQUEUEOPERATE ||
//				nMsgID == MSGID_CS_PUBLICHOUSE_OPERATE ||
//				nMsgID == MSGID_CS_JOIN_CLAN)
//			{
//				bCanDo = true;
//			}
//		} break;
//		// 玩家在别人的基地中
//	case GAME_STATE_IN_OTHER_BASE:	
//		{
//
//		} break;
//		// 玩家在地图上
//	case GAME_STATE_ON_MAP:	
//		{
//
//		} break;
//		// 玩家在战斗中
//	case GAME_STATE_COMBAT:	
//		{
//
//		} break;
//		// 玩家在藏宝中
//	case GAME_STATE_HIDE:	
//		{
//
//		} break;
//	default:
//		{
//			CacheLog_ERROR(LOG_ERROR, "wrong game state:%d in Player::canDo()", m_nGameState);
//		} break;
//	}
//
//	return bCanDo;
//	__LEAVE_FUNCTION__
//}

void Player::gameState(GameState val)
{
	__ENTER_FUNCTION__
		m_nGameState = val;

	switch (val)
	{
		// 等待登陆（过了login server）
	case GAME_STATE_WAIT_LOGIN: 
		{
			m_bOnline = true;

		} break;
		// 登陆通过了，等待玩家选择去哪
	case GAME_STATE_LOGIN_PASSED: 
		{

		} break;
		// 玩家在自己的基地中
	case GAME_STATE_IN_SELF_BASE:	
		{

		} break;
		// 玩家在别人的基地中
	case GAME_STATE_IN_OTHER_BASE:	
		{

		} break;
		// 玩家在地图上
	case GAME_STATE_ON_MAP:	
		{

		} break;
		// 玩家在战斗中
	case GAME_STATE_COMBAT:	
		{

		} break;
		// 玩家在藏宝中
	case GAME_STATE_HIDE:	
		{

		} break;
	case GAME_STATE_LOGOUT:
		{
			m_bOnline = false;
		} break;
	default:
		{
			CacheLog_ERROR(LOG_ERROR, "change to wrong game state:%d", val);
		} break;
	}
	__LEAVE_FUNCTION__
}

bool Player::inClan() const
{
	__ENTER_FUNCTION__
		bool bIn = false;
	if (m_playerInfo.m_nClanGuid != invalid_guid){
		bIn = true;
	}

	return bIn;
	__LEAVE_FUNCTION__
}

void Player::name( const char* szPlayerName )
{
	__ENTER_FUNCTION__
		Assert(szPlayerName != NULL && *szPlayerName != '\0');
	strncpy(m_playerInfo.m_szName, szPlayerName, PLAYER_NAME_WITH_CLOSING);
	m_playerInfo.m_szName[PLAYER_NAME_WITH_CLOSING - 1] = 0;

	setPlayerInfoChange(PLAYER_INFO_CHANGE_NAME);
	__LEAVE_FUNCTION__
}

void Player::addMail( const MessageMail& mail )
{
	__ENTER_FUNCTION__
		m_mailMgr.add(mail);
	__LEAVE_FUNCTION__
}

void Player::removeRequestMail( unsigned char byType, Guid_t nRequestPlayerGuid )
{
	__ENTER_FUNCTION__
		m_mailMgr.removeRequestMail(byType, nRequestPlayerGuid);
	__LEAVE_FUNCTION__
}

void Player::removeAllRequestMail( unsigned char byType )
{
	__ENTER_FUNCTION__
		m_mailMgr.removeAllRequestMail(byType);
	__LEAVE_FUNCTION__
}

void Player::loadAchievementsFromDB()
{
	__ENTER_FUNCTION__
    // TODO:??? TEST
    int nCount = AchievementsConfig::getSingleton()->getCount();
    for (int i = 0; i < nCount; i++)
    {
        AchievementItem item;
        item.id = i;
        item.progress = 0;
        m_playerInfo.m_achievements.AddAchievementItem(item);
    }
	__LEAVE_FUNCTION__
}

const int Player::getExp() const
{
	__ENTER_FUNCTION__
	return m_playerInfo.m_nExp;
	__LEAVE_FUNCTION__
}

const int Player::getNowLevel() const
{
	__ENTER_FUNCTION__
	return m_playerInfo.m_nLevel;
	__LEAVE_FUNCTION__
}

const int Player::getNextLevelNeedExp() const
{
	__ENTER_FUNCTION__
	return m_playerInfo.m_nNextLevelNeedExp;
	__LEAVE_FUNCTION__
}

void Player::addExp(const int nExp)
{
	__ENTER_FUNCTION__
	Assert(nExp >= 0);
	if (m_playerInfo.m_nExp + nExp >= m_playerInfo.m_nNextLevelNeedExp)
	{
		m_playerInfo.m_nExp = (m_playerInfo.m_nExp+nExp)-m_playerInfo.m_nNextLevelNeedExp;
		Levelup();
	}
	else
	{
		m_playerInfo.m_nExp += nExp;
	}
	setPlayerInfoChange(PLAYER_INFO_CHANGE_EXP);
	__LEAVE_FUNCTION__
}

const int Player::getIconID() const
{
	__ENTER_FUNCTION__
	return m_playerInfo.m_nIconID;
	__LEAVE_FUNCTION__
}

void Player::setNowIconID(const int nIconID)
{
	__ENTER_FUNCTION__
	m_playerInfo.m_nIconID = nIconID;
	setPlayerInfoChange(PLAYER_INFO_CHANGE_ICON);
	__LEAVE_FUNCTION__
}

const int Player::getMoneyNum() const
{
	__ENTER_FUNCTION__
	return m_playerInfo.m_nMoneyValue;
	__LEAVE_FUNCTION__
}

void Player::addMoneyNum(const int nNum)
{
	__ENTER_FUNCTION__
	Assert(nNum >= 0);
	m_playerInfo.m_nMoneyValue += nNum;
	setPlayerInfoChange(PLAYER_INFO_CHANGE_MONEY);
	__LEAVE_FUNCTION__
}

bool Player::consumeMoneyNum(const int nNum)
{
	__ENTER_FUNCTION__
	Assert(nNum >= 0);
	if (m_playerInfo.m_nMoneyValue < nNum)
	{
		return false;
	}
	m_playerInfo.m_nMoneyValue -= nNum;

	setPlayerInfoChange(PLAYER_INFO_CHANGE_MONEY);
	return true;
	__LEAVE_FUNCTION__
}

void Player::Levelup()
{
	__ENTER_FUNCTION__
	Assert(m_playerInfo.m_nLevel > 0);
	CDataTable* pConfigTable = CTableSystem::GetInstancePtr()->GetTableById(TABLE_PLAYER_LEVEL_CONFIG);
	Assert(pConfigTable != NULL);

	int nMaxLevel = pConfigTable->GetLineNum();
	if (m_playerInfo.m_nLevel >= nMaxLevel)
	{
		CacheLog_DEBUG(LOG_ERROR, "Player::Levelup player have reach maxlevel, playerguid: %d, playername: %s", this->getGuid(), this->name());
		return;
	}

	_DBC_T_PLAYER_LEVEL_CONFIG* pConfigLine = (_DBC_T_PLAYER_LEVEL_CONFIG*)pConfigTable->GetlineById(m_playerInfo.m_nLevel+1);
	Assert(pConfigLine);

	m_playerInfo.m_nLevel += 1;
	m_playerInfo.m_nNextLevelNeedExp = pConfigLine->nLevelupNeedExp;

	// 一次获得的经验太多了
	if (m_playerInfo.m_nExp >= m_playerInfo.m_nNextLevelNeedExp)
	{
		CacheLog_DEBUG(LOG_ERROR, "Player::Levelup get exp so many, shold checkis, playerguid: %d, playername: %s, now level: %d, now exp: %d", 
			this->getGuid(), this->name(), this->getNowLevel(), this->getExp() );
		m_playerInfo.m_nExp -= m_playerInfo.m_nNextLevelNeedExp;
		Levelup();
	}

	setPlayerInfoChange(PLAYER_INFO_CHANGE_LEVEL);
	setPlayerInfoChange(PLAYER_INFO_CHANGE_EXP);


	__LEAVE_FUNCTION__
}

void Player::HeartBeat(float nowTime, float dt)
{
	validatePlayerInfoChange(dt);
}

void Player::setPlayerInfoChange(const PLAYER_INFO_CHANGE_INDEX dataindex){
	m_nPlayerInfoChangeFlag |= dataindex;
}

void Player::validatePlayerInfoChange(float dt)
{
	if (m_nPlayerInfoChangeFlag > 0)
	{
		// 向关心的客户端发送变更消息
		SCPlayerInfoChange msgPlayerInfoChange;
		msgPlayerInfoChange.SetDataChangeFlag(m_nPlayerInfoChangeFlag);
		msgPlayerInfoChange.targetGuid(m_playerInfo.m_nPlayerGuid);
		msgPlayerInfoChange.playerInfo(m_playerInfo);
		PersonalBase* pPersonal = PersonalBaseManager::GetInstancePtr()->getPersonalBaseByGuid(m_playerInfo.m_nPlayerGuid);
		if (pPersonal)
		{
			pPersonal->BroadcastMsg(&msgPlayerInfoChange);
		}

		// 通知DB保存信息
		DBCommand_Player_Save* pSavePlayerDBCommand = (DBCommand_Player_Save*)DBCommandCreator::CreateDBCommand(DBCOMMAND_TYPE_PLAYER_SAVE);
		Assert(pSavePlayerDBCommand != NULL);
		pSavePlayerDBCommand->InitDBCommand(this->playerInfo(), m_nPlayerInfoChangeFlag);
		DBCommandPool::GetInstancePtr()->AddCommonCommand(pSavePlayerDBCommand);

		m_nPlayerInfoChangeFlag = 0;
	}
}

void Player::UseTitleOfAchievementId( const int nAchieveId )
{
    if (m_playerInfo.m_nUseTitleOfAchievementId != nAchieveId)
    {
        m_playerInfo.m_nUseTitleOfAchievementId = nAchieveId;
        setPlayerInfoChange(PLAYER_INFO_CHANGE_ACHIEVEMENT_TITLE);
    }
}

void Player::clanName( const char* val )
{
	__ENTER_FUNCTION__
	strncpy(m_playerInfo.m_szClanName, val, CLAN_NAME_WITH_CLOSING);
	m_playerInfo.m_szClanName[CLAN_NAME_WITH_CLOSING - 1] = 0;
	__LEAVE_FUNCTION__
}
