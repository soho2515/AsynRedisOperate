#include "MsgHandler.h"
#include "AllPacketInclude.h"
#include "ClientConnection.h"
#include "PlayerDataServer.h"
#include "../../Public/Packets/ServerLoginMsg.h"

#include "PersonalBaseManager.h"
#include "PlayerManager.h"
#include "BuildingConfig.h"
#include "DataTable.h"
#include "TableSystem.h"
#include "GameUtility.h"
#include <algorithm>
#include "AchievementsConfig.h"
#include "redisdb/DBCommandCreator.h"
#include "redisdb/DBCommandPool.h"
#include "redisdb/DBCommandCallBackPool.h"
#include "DBCommand_Player.h"
#include "DBCommandCallback_Player.h"

namespace MSG_HANDLER{

	MSG_HANDLER_RETURN onHeartBeat(IMsgHandler* pMsgHandler, Packet* pMsg)
	{
		__ENTER_FUNCTION__
			return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__
	}

	MSG_HANDLER_RETURN onAskPlayerInfo(IMsgHandler* pMsgHandler, Packet* pMsg)
	{
		__ENTER_FUNCTION__
			ClientConnection* pClient = (ClientConnection*)pMsgHandler;

		CSAskPlayerInfo* pAskPlayerInfo = (CSAskPlayerInfo*)pMsg;

		Player* pPlayer = NULL;
		pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pAskPlayerInfo->targetGuid());
		// 玩家信息没有加载
		if (pPlayer == NULL){
			// 数据库加载数据
			DBCommand_Player_Load* pLoadPlayerDBCommand = (DBCommand_Player_Load*)DBCommandCreator::CreateDBCommand(DBCOMMAND_TYPE_PLAYER_LOAD);
			Assert(pLoadPlayerDBCommand != NULL);
			pLoadPlayerDBCommand->InitDBCommand(pAskPlayerInfo->targetGuid());
			DBCommandCallback_AskPlayerInfo* pAskPlayerDBCallBack = (DBCommandCallback_AskPlayerInfo*)DBCommandCallbackCreator::CreateDBCommandCallback(DBCOMMAND_CALLBACK_ASKPLAYERINFO);
			Assert(pAskPlayerDBCallBack);
			pAskPlayerDBCallBack->Init(pAskPlayerInfo->visitorGUID(), pAskPlayerInfo->targetGuid(), pClient->getConnectionID());
			pLoadPlayerDBCommand->setCallbackID(DBCommandCallBackPool::GetInstancePtr()->AddDBCommandCallBack(pAskPlayerDBCallBack));
			DBCommandPool::GetInstancePtr()->AddPriorityCommand(pLoadPlayerDBCommand);
		}
		else
		{
			pPlayer->SendPlayerInfo(pClient, pAskPlayerInfo->visitorGUID());
			if (pAskPlayerInfo->visitorGUID() == pAskPlayerInfo->targetGuid())
			{
				pPlayer->SetSelfGameServerConnection(pClient); 
			}
		}

		// !! TO DO [1/16/2013 dujiang] 将来要让玩家自己输入自己的昵称，现在仅是模拟
		/*
		const int NAME_LEN = 8;
		char szRandName[NAME_LEN] = "love147";
		std::random_shuffle(szRandName, szRandName + NAME_LEN - 1);
		pPlayer->name(szRandName);

		pPlayer->gameState(GAME_STATE_LOGIN_PASSED);
		*/

		return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__
	}

	MSG_HANDLER_RETURN onAskEnterPersonalBase( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		__ENTER_FUNCTION__
			ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
		CSAskEnterPersonalBase* pAskMsg = (CSAskEnterPersonalBase*)pMsg;

		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pAskMsg->visitorID());
		// !! TO DO [12/14/2012 dujiang] 目前player的信息是一起来就载入到内存中的
		Assert(pPlayer != NULL);
		if (pPlayer->getGuid() == pAskMsg->targetID()){
			pPlayer->gameState(GAME_STATE_IN_SELF_BASE);
		}
		else{
			pPlayer->gameState(GAME_STATE_IN_OTHER_BASE);
		}

		PersonalBase* pBase = PersonalBaseManager::GetInstancePtr()->getPersonalBaseByGuid(pAskMsg->targetID());
		// 目前此基地信息已在内存中
		if (pBase != NULL){
			SCEnterPersonalBaseRet retMsg;
			retMsg.targetGuid(pAskMsg->targetID());
			retMsg.visitorGuid(pAskMsg->visitorID());
			retMsg.SetResult(SCEnterPersonalBaseRet::EPBR_SUCCEEDED);
			pClientConnection->SendMsg(&retMsg);

			// !! TO DO [12/5/2012 dujiang] gameserver要发消息过来注销此关注
			pBase->BeginNewVistor(pClientConnection, pAskMsg->visitorID());
		}
		else
		{
			DBCommand_Player_Load* pLoadPlayerDBCommand = (DBCommand_Player_Load*)DBCommandCreator::CreateDBCommand(DBCOMMAND_TYPE_PLAYER_LOAD);
			Assert(pLoadPlayerDBCommand != NULL);
			pLoadPlayerDBCommand->InitDBCommand(pAskMsg->targetID());
			DBCommandCallback_AskPlayerInfo* pAskPlayerDBCallBack = (DBCommandCallback_AskPlayerInfo*)DBCommandCallbackCreator::CreateDBCommandCallback(DBCOMMAND_CALLBACK_ASKPLAYERINFO);
			Assert(pAskPlayerDBCallBack);
			pAskPlayerDBCallBack->Init(pAskMsg->visitorID(), pAskMsg->targetID(), pClientConnection->getConnectionID(), true);
			pLoadPlayerDBCommand->setCallbackID(DBCommandCallBackPool::GetInstancePtr()->AddDBCommandCallBack(pAskPlayerDBCallBack));
			DBCommandPool::GetInstancePtr()->AddPriorityCommand(pLoadPlayerDBCommand);
		}

		return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onGDUnregVisitor( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		__ENTER_FUNCTION__
			ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
		GDUnregVisitor* pAskMsg = (GDUnregVisitor*)pMsg;
		PersonalBase* pBase = PersonalBaseManager::GetInstancePtr()->getPersonalBaseByGuid(pAskMsg->TargetID());
		if (pBase == NULL){
			CacheLog_ERROR(LOG_ERROR, "unreg visitor guid:%d to target player guid:%d", pAskMsg->VisitorID(), pAskMsg->TargetID());
		}
		else{
			pBase->RemoveVisitor(pAskMsg->VisitorID(), pClientConnection);
		}

		return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onCSCreateBuilding( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		__ENTER_FUNCTION__
			ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
		CSCreateBuilding* pCreateMsg = (CSCreateBuilding*)pMsg;

		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pCreateMsg->ownerGuid());
		// 玩家只能在自己的基地中建设
		Assert(pPlayer != NULL && pPlayer->getGuid() == pCreateMsg->ownerGuid());

		//bool bCanDo = pPlayer->canDo(pCreateMsg->GetPacketID());
			bool bCanDo = canDo(pPlayer->gameState(), pCreateMsg->GetPacketID());
		if (!bCanDo){
			CacheLog_DEBUG(LOG_DEBUG, "player(guid:%d, game state:%d), can not do this in onCSCreateBuilding()",
				pPlayer->getGuid(), pPlayer->gameState());
			return MSG_HANDLER_SUCCESS;
		}

		PersonalBase* pBase = PersonalBaseManager::GetInstancePtr()->getPersonalBaseByGuid(pCreateMsg->ownerGuid());
		Assert(pBase != NULL);

		// 如果是建设在底座上，则先判断底座上是否有建筑
		// 判断是否到了建筑上限，钱是否够。。。
		Building* pNewBuilding = pBase->CreateNewBuilding(pCreateMsg->GetTblID(), pCreateMsg->GetTag(), pCreateMsg->GetHelperGuid());

		if (pNewBuilding != NULL)
		{
			pNewBuilding->nPosX = pCreateMsg->GetPosX();
			pNewBuilding->nPosY = pCreateMsg->GetPosY();

			SCCreateBuilding msg;
			msg.ownerGuid(pPlayer->getGuid());
			msg.setBuildingData(pNewBuilding);
			pClientConnection->SendMsg(&msg);
		}
		else
		{
			// 给玩家提示
			CacheLog_WARNNING(LOG_DEBUG, "onCSCreateBuilding: CreateNewBuilding return NULL guid=%d", pPlayer->getGuid());
		}

		return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__

	}

	extern MSG_HANDLER_RETURN onCSBuildingOperation( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		__ENTER_FUNCTION__
			ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
			CSBuildingOperation* pBuildingMsg = (CSBuildingOperation*)pMsg;
	
			Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pBuildingMsg->OwnerGuid());
			// 玩家只能在自己的基地中升级建筑
			Assert(pPlayer != NULL && pPlayer->getGuid() == pBuildingMsg->OwnerGuid());
	
			//bool bCanDo = pPlayer->canDo(pBuildingMsg->GetPacketID());
			bool bCanDo = canDo(pPlayer->gameState(), pBuildingMsg->GetPacketID());
			if (!bCanDo){
				CacheLog_DEBUG(LOG_DEBUG, "player(guid:%d, game state:%d), can not do this in onCSBuildingOperation()",
					pPlayer->getGuid(), pPlayer->gameState());
				return MSG_HANDLER_SUCCESS;
			}
			
			PersonalBase* pBase = PersonalBaseManager::GetInstancePtr()->getPersonalBaseByGuid(pBuildingMsg->OwnerGuid());
			Assert(pBase != NULL);
			Assert(pPlayer->getGuid() == pBase->GetOwnerGUID());
	
			int nBuildingObjId = pBuildingMsg->GetObjID();
			Building* pBuilding = pBase->GetBuildingById(nBuildingObjId);
			if (pBuilding == NULL)
			{
				CacheLog_ERROR(LOG_ERROR, "error!! player send a invalid building objid in CSBuildingOperation!");
			return MSG_HANDLER_SUCCESS;
			}
	
			switch(pBuildingMsg->GetOperateType())
			{
			case CSBuildingOperation::BO_MoveTo:
				{
					pBuilding->nPosX = pBuildingMsg->GetParam1();
					pBuilding->nPosY = pBuildingMsg->GetParam2();
					CacheLog_DEBUG(LOG_DEBUG, "Building move to (%d,%d)", pBuilding->nPosX, pBuilding->nPosY);
				}
				break;
			case CSBuildingOperation::BO_Destroy:
				{
					//// 1. 判断是否可拆除 
					//if (BuildingConfig::getSingleton()->getBaseCfg(pBuilding->nTableId)->nDestroyable == 0)
					//{
					//	return MSG_HANDLER_SUCCESS;
					//}
					//// 查表
					//// 2. 数据存储修改
					//if (pBase->DestroyBuildingByID(nBuildingObjId))
					//{
					//	SCSendBuildInfo msg;
					//	msg.SetObjID(nBuildingObjId);
					//	msg.SetInfoType(SCSendBuildInfo::SBI_Destroyed);
					//	msg.visitorGuid(pBase->GetOwnerGUID());
					//	pClientConnection->SendMsg(&msg);
					//}
				}
				break;
			case CSBuildingOperation::BO_Upgrade:
				{
					int nMaxGrade = BuildingConfig::getSingleton()->getLevelCount(pBuilding->nTableId);
					if (pBuilding->nLevel >= nMaxGrade 
						|| pBuilding->nState == Building::BS_Upgrade
						|| pBuilding->nState == Building::BS_Build
						)
					{
						return MSG_HANDLER_SUCCESS;
					}
					_DBC_T_BUILDINGLEVELCFG* pBLC = BuildingConfig::getSingleton()->getLevelCfg(pBuilding->nTableId, pBuilding->nLevel);
					int nHelperGuid = pBuildingMsg->GetHelperGuid();
					// 1. 升级判断
					int nCostCoin = pBLC->nCostGoldcoinForUpgrade;
					int nCostEnergy = pBLC->nCostEnergyForUpgrade;
					int nCostGem = pBLC->nCostGemForUpgrade;
					int nHallLevelNeed = pBLC->nMainLevelRequirement;
					// 主建筑等级约束
					if (pBase->GetHallLevel() < nHallLevelNeed)
					{
						CacheLog_DEBUG(LOG_DEBUG, "Error! need hall level=%d", nHallLevelNeed);
						return MSG_HANDLER_SUCCESS;
					}
					// 消耗判断
					if (nCostCoin > pBase->GetGoldCoin() || nCostEnergy > pBase->GetEnergy() || nCostGem > pBase->GetGemCount())
					{
						CacheLog_DEBUG(LOG_DEBUG, "upgrade failed! ressource is not enough!"); 
						return MSG_HANDLER_SUCCESS;
					}

					if (nHelperGuid != invalid_guid)
					{
						if ( !pBase->CanHireFriend(nHelperGuid) )
						{
							return MSG_HANDLER_SUCCESS;
						}

						pBase->HireFriend(nHelperGuid);
					}
					else
					{
						if (pBase->GetWorkingBuilderCount() < pBase->GetBuilderCount())// 工人数量限制
						{
							pBase->SetWorkingBuilderCount(pBase->GetWorkingBuilderCount()+1);
						}
						else
						{
							//TODO: 提示
							return MSG_HANDLER_SUCCESS;
						}
					}

					// 2. 升级消耗

					pBase->CostGold(nCostCoin);
					pBase->CostEnergy(nCostEnergy);
					pBase->CostGem(nCostGem);
						
					// 3. 升级
					pBuilding->nState = Building::BS_Upgrade;
					pBuilding->nBeginTime = CTimeSystem::Instance()->GetBigSecond();
					pBuilding->nLeftTime = pBLC->nUpgradeTime;
					// 发消息告诉客户端开始升级
					SCSendBuildInfo msg;
					msg.SetObjID(nBuildingObjId);
					msg.SetInfoType(SCSendBuildInfo::SBI_UpgradeStart);
					msg.visitorGuid(pBase->GetOwnerGUID());
					pClientConnection->SendMsg(&msg);
				}
				break;
			case CSBuildingOperation::BO_Cancel:
				{
					// TODO:??? 取消升级或者建设,归还50%的费用                    
					if (pBuilding->nState == Building::BS_Upgrade)
					{
						pBuilding->nState = Building::BS_Idle;

						_DBC_T_BUILDINGLEVELCFG* pBLC = BuildingConfig::getSingleton()->getLevelCfg(pBuilding->nTableId, pBuilding->nLevel);
						int nCostCoin = pBLC->nCostGoldcoinForUpgrade;
						int nCostEnergy = pBLC->nCostEnergyForUpgrade;
						pBase->AddGold(nCostCoin*CANCEL_BUILD_OR_UPGRADE_COST_FEEDBACK_RATE);
						pBase->AddEnergy(nCostEnergy*CANCEL_BUILD_OR_UPGRADE_COST_FEEDBACK_RATE);

						SCSendBuildInfo msg;
						msg.SetObjID(nBuildingObjId);
						msg.visitorGuid(pBase->GetOwnerGUID());
						msg.SetInfoType(SCSendBuildInfo::SBI_UpgradeCanceled);
						pClientConnection->SendMsg(&msg);
					}
					else if(pBuilding->nState == Building::BS_Build)
					{
						_DBC_T_BASEBUILDING* pCfg = BuildingConfig::getSingleton()->getBaseCfg(pBuilding->nTableId);
						pBase->AddGold(pCfg->nCostGoldForBuild*CANCEL_BUILD_OR_UPGRADE_COST_FEEDBACK_RATE);
						pBase->AddEnergy(pCfg->nCostEnergyForBuild*CANCEL_BUILD_OR_UPGRADE_COST_FEEDBACK_RATE);

						if (pBase->DestroyBuildingByID(nBuildingObjId))
						{
							SCSendBuildInfo msg;
							msg.SetObjID(nBuildingObjId);
							msg.SetInfoType(SCSendBuildInfo::SBI_Destroyed);
							msg.visitorGuid(pBase->GetOwnerGUID());
							pClientConnection->SendMsg(&msg);
						}
					}
				}
				break;
			case CSBuildingOperation::BO_CompleteNow:
				{
					int nGem = gTime2Gem(pBuilding->nLeftTime);
					if (nGem > pBase->GetGemCount())
					{
						// TODO:??? 用severcode返回客户端
						return MSG_HANDLER_SUCCESS;
					}
					pBase->CostGem(nGem);
					if (pBuilding->nState == Building::BS_Upgrade)
					{
						pBase->OnBuildingUpgradeCompleted(*pBuilding);
					}
					else if(pBuilding->nState == Building::BS_Build)
					{
						pBase->OnBuildingBuildCompleted(*pBuilding);
					}
					pBuilding->nState = Building::BS_Idle;
					pBuilding->nLeftTime = 0;
				}
				break;
			case CSBuildingOperation::BO_Crop:
				if (pBuilding->nState == Building::BS_ResFull || pBuilding->nState == Building::BS_Collect)
				{
					int nCropValue = pBuilding->CollectionExtDataRT.nResNumber;
					if (pBuilding->nLogicType == BTYPE_GOLDMINE)
					{
						if (pBase->GetGoldCoin() + nCropValue > pBase->GetMaxGoldCoin())
						{
							//TODO:??? 提示玩家需要升级仓库
						}
						pBase->AddGold(nCropValue);
					}
					else if (pBuilding->nLogicType == BTYPE_ENERGY_PRODUCE_HOUSE)
					{
						if (pBase->GetEnergy() + nCropValue > pBase->GetMaxEnergy())
						{
							
						}
						pBase->AddEnergy(nCropValue);
					}
					
					pBuilding->CollectionExtDataRT.nResNumber = 0;

					pBuilding->nState = Building::BS_Collect;
					pBuilding->nBeginTime = CTimeSystem::Instance()->GetBigSecond();
					pBuilding->nLeftTime = (float)pBuilding->CollectionExt.nCapacity / (float)pBuilding->CollectionExt.nProductionRate * 3600.f;

					SCSendBuildInfo msg;
					msg.SetObjID(nBuildingObjId);
					msg.SetInfoType(SCSendBuildInfo::SBI_CropCompleted);
					msg.visitorGuid(pBase->GetOwnerGUID());
					msg.SetParam1(nCropValue);
					pClientConnection->SendMsg(&msg);//TODO:???改变状态，并通知玩家,需要广播么？
				}
	
				break;
			default:
				break;
			}
	
			return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__
	}

	MSG_HANDLER_RETURN onCSPublicHouseOperate( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
		CSPublicHouseOperate* pPubMsg = (CSPublicHouseOperate *)pMsg;

		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pPubMsg->OwnerGuid());
		// 玩家只能在自己的基地中操作怪物招募
		if (pPlayer != NULL && pPlayer->getGuid() == pPubMsg->OwnerGuid())
		{
		}
		else
		{
			return MSG_HANDLER_SUCCESS;    
		}

		//bool bCanDo = pPlayer->canDo(pPubMsg->GetPacketID());
		bool bCanDo = canDo(pPlayer->gameState(), pPubMsg->GetPacketID());
		if (!bCanDo){
			CacheLog_DEBUG(LOG_DEBUG, "player(guid:%d, game state:%d), can not do this in onCSPublicHouseOperate()",
				pPlayer->getGuid(), pPlayer->gameState());
			return MSG_HANDLER_SUCCESS;
		}

		PersonalBase* pBase = PersonalBaseManager::GetInstancePtr()->getPersonalBaseByGuid(pPubMsg->OwnerGuid());
		Assert(pBase != NULL);
		Assert(pPlayer->getGuid() == pBase->GetOwnerGUID());

		int nBuildingObjId = pPubMsg->GetObjIdOfPub();
		Building* pBuilding = pBase->GetBuildingById(nBuildingObjId);

		switch (pPubMsg->GetOperateType())
		{
		case CSPublicHouseOperate::PHO_RecruitOne:
			{
				if (pPubMsg->GetBornPointIdx() < 0 || pPubMsg->GetBornPointIdx() >= MIN(PUB_MAX_MONSTER_BORNPOINT_COUNT, pBuilding->PublicHouseExt.nMonsterBornPointsCount))
				{
					return MSG_HANDLER_SUCCESS;
				}
				card_id cid = pBuilding->PublicHouseExtDataRT.arrMonsters[pPubMsg->GetBornPointIdx()];
				if (cid == invalid_card_id)
				{
					return MSG_HANDLER_SUCCESS;
				}
				CDataTable* pEnemyTable = CTableSystem::GetInstancePtr()->GetTableById(TABLE_ENEMY);
				Assert(pEnemyTable);
				_DBC_T_ENEMY* pEnemyLine = (_DBC_T_ENEMY*)pEnemyTable->GetlineById(cid);
				if (pEnemyLine == NULL)
				{
					// TODO:??? 
					return MSG_HANDLER_SUCCESS;
				}
				Assert(pEnemyLine != NULL) 
					if (pBase->GetGoldCoin() < pEnemyLine->nRecruitCostCoin)
					{
						//TODO:??? 提醒客户端金币不够
					}
					else
					{
						pBase->CostGold(pEnemyLine->nRecruitCostCoin);
						pBuilding->PublicHouseExtDataRT.arrMonsters[pPubMsg->GetBornPointIdx()] = invalid_card_id; // 删除怪物
					}
			}
			break;
		case CSPublicHouseOperate::PHO_RecruitAll:
			{
				// TODO:??? 先判断总消耗是否够
			}
			break;
		case CSPublicHouseOperate::PHO_RefreshNow:
			{
				int nGemNeed = gTime2Gem(pBuilding->nLeftTime);
				if (nGemNeed < pBase->GetGemCount())
				{
					CacheLog_DEBUG(LOG_DEBUG, "Gem is not enough!");
				}
				else
				{
					pBuilding->nBeginTime = 0; // 修改开始时间造成立即刷新
				}
			}
			break;
		default:
			break;
		}
		return MSG_HANDLER_SUCCESS;
	}

	extern MSG_HANDLER_RETURN onCSUpgradeOperate(IMsgHandler* pMsgHandler, Packet* pMsg){
		__ENTER_FUNCTION__

			ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
		CSUpgradeOperate* pUpgradeOperate = (CSUpgradeOperate *)pMsg;

		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pUpgradeOperate->OwnerGuid());
		// 玩家只能在自己的基地中操作怪物招募
		if (pPlayer == NULL || pPlayer->getGuid() != pUpgradeOperate->OwnerGuid())
		{
			CacheLog_DEBUG(LOG_ERROR, "onCSUpgradeOperate pPlayer error, guid: %d", pPlayer == NULL ? -1 : pPlayer->getGuid());
			return MSG_HANDLER_SUCCESS;
		}

		PersonalBase* pPersonalBase = PersonalBaseManager::GetInstancePtr()->getPersonalBaseByGuid(pPlayer->getGuid());
		if (pPersonalBase == NULL)
		{
			CacheLog_DEBUG(LOG_ERROR, "onCSUpgradeOperate error, cannot find personalbase, guid: %d", pPlayer->getGuid());
			return MSG_HANDLER_SUCCESS;
		}
		CacheLog_DEBUG(LOG_DEBUG, "onCSUpgradeOperate upgrade operate, playerguid is %d, operatetype is %d, skilltblindex is : %d", pPlayer->getGuid(), pUpgradeOperate->GetOperateType(), pUpgradeOperate->GetTargetTblIndex());
		return MSG_HANDLER_SUCCESS;

		__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onCSCreateClan(IMsgHandler* pMsgHandler, Packet* pMsg)
	{
		ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
		CSCreateClan* pCreateMsg = (CSCreateClan*)pMsg;

		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pCreateMsg->creator().nPlayerGuid);
		// 玩家只能在自己的基地中
		Assert(pPlayer != NULL && pPlayer->getGuid() == pCreateMsg->creator().nPlayerGuid);
		Assert(pCreateMsg->creator().nPlayerGuid != invalid_guid);
		//Assert(pCreateMsg->creator().szName != '\0' && strlen(pCreateMsg->creator().szName) > 0);
		Assert(pCreateMsg->clanProperties().clanName() != '\0' && 
			strlen(pCreateMsg->clanProperties().clanName()) > 0);

		//bool bCanDo = pPlayer->canDo(pCreateMsg->GetPacketID());
		bool bCanDo = canDo(pPlayer->gameState(), pCreateMsg->GetPacketID());
		if (!bCanDo){
			CacheLog_ERROR(LOG_ERROR, "player(guid:%d, game state:%d), can not do this in onCSCreateClan()",
				pPlayer->getGuid(), pPlayer->gameState());
			return MSG_HANDLER_SUCCESS;
		}

		bool bInClan = pPlayer->inClan();
		if (bInClan){
			SCCreateClanRet retMsg;
			retMsg.createResult(RETURN_VALUE_PLAYER_ALREADY_IN_CLAN);
			retMsg.creatorGuid(pPlayer->getGuid());
			retMsg.clanProperties(pCreateMsg->clanProperties());

			pClientConnection->SendMsg(&retMsg);
		}
		else{
			g_pPlayerDataServer->getWorldServerConnector()->SendMsg(pCreateMsg);
		}

		CacheLog_DEBUG(LOG_DEBUG, "onCSCreateClan, player guid:%d, clan name:%s",
			pCreateMsg->creator().nPlayerGuid, pCreateMsg->clanProperties().clanName());

		return MSG_HANDLER_SUCCESS;
	}

	extern MSG_HANDLER_RETURN onSCCreateClanRet( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		__ENTER_FUNCTION__
			SCCreateClanRet *pRetMsg = (SCCreateClanRet*)pMsg;

		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pRetMsg->creatorGuid());
		Assert(pPlayer != NULL);
		Assert(pPlayer->getGuid() == pRetMsg->creatorGuid());

		if (pRetMsg->createResult() == RETURN_VALUE_OK){
			pPlayer->clanGuid(pRetMsg->clanProperties().clanGuid());
			pPlayer->clanSymbolID(pRetMsg->clanProperties().clanSymbol());
			pPlayer->clanName(pRetMsg->clanProperties().clanName());
		}

		g_pPlayerDataServer->sendPlayerMsgToGameServer(pPlayer->getGuid(), pRetMsg);
		CacheLog_DEBUG(LOG_DEBUG, "create result:%d, onSCCreateClanRet", pRetMsg->createResult());

		return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__
	}


	extern MSG_HANDLER_RETURN onSCLeaveClanRet(IMsgHandler* pMsgHandler, Packet* pMsg)
	{
		__ENTER_FUNCTION__
		// worldserver处理完后传过来的
		SCLeaveClanRet* pLeaveRet = (SCLeaveClanRet*)pMsg;
		CacheLog_DEBUG(LOG_DEBUG, "onSCLeaveClanRet(), ret value:%d, clan guid:%d, player guid:%d", 
			pLeaveRet->returnValue(), pLeaveRet->clanGuid(), pLeaveRet->playerGuid());

		if (pLeaveRet->returnValue() == RETURN_VALUE_OK){
			Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pLeaveRet->playerGuid());
			Assert(pPlayer != NULL);
			Assert(pPlayer->getGuid() == pLeaveRet->playerGuid());
			pPlayer->clanGuid(invalid_guid);
			pPlayer->removeAllRequestMail(MAIL_TYPE_JOIN_CLAN_APPLICATION);
		}

		g_pPlayerDataServer->sendPlayerMsgToGameServer(pLeaveRet->playerGuid(), pLeaveRet);

		return MSG_HANDLER_SUCCESS;
			__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onSCKickoutClanMemberRet( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		__ENTER_FUNCTION__
		// worldserver处理完后传过来的
		SCKickoutClanMemberRet* pRetMsg = (SCKickoutClanMemberRet*)pMsg;
		// 是被踢的人
		if (pRetMsg->sendToPlayerGuid() == pRetMsg->targetPlayerGuid()){
			Assert(pRetMsg->returnValue() == RETURN_VALUE_OK);

			Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pRetMsg->sendToPlayerGuid());
			if (pPlayer == NULL){
				// !! TO DO [2/15/2013 dujiang] need to load from DB
			}
			else{
				Assert(pPlayer->getGuid() == pRetMsg->targetPlayerGuid());
				pPlayer->clanGuid(invalid_guid);
			}
		}

		g_pPlayerDataServer->sendPlayerMsgToGameServer(pRetMsg->sendToPlayerGuid(), pRetMsg);

		return MSG_HANDLER_SUCCESS;
			__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onSCPromoteClanMemberRet( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		__ENTER_FUNCTION__
		// worldserver处理完后传过来的
		SCPromoteClanMemberRet* pRetMsg = (SCPromoteClanMemberRet*)pMsg;

		if (pRetMsg->returnValue() == RETURN_VALUE_OK){
			if (pRetMsg->sendToPlayerGuid() == pRetMsg->sourcePlayerGuid()){
				if (pRetMsg->leaderChanged()){
					Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pRetMsg->sendToPlayerGuid());
					// !! TO DO [13/1/2013 dujiang] 不在内存中需另作处理
					if (pPlayer != NULL){
						pPlayer->removeAllRequestMail(MAIL_TYPE_JOIN_CLAN_APPLICATION);
					}
				}
			}
		}
		g_pPlayerDataServer->sendPlayerMsgToGameServer(pRetMsg->sendToPlayerGuid(), pRetMsg);
		return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__
	} 

	extern MSG_HANDLER_RETURN onSCDemoteClanMemberRet( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		__ENTER_FUNCTION__
		// worldserver处理完后传过来的
		SCDemoteClanMemberRet* pRetMsg = (SCDemoteClanMemberRet*)pMsg;

		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pRetMsg->sendToPlayerGuid());
		// !! TO DO [13/1/2013 dujiang] 不在内存中需另作处理
		if (pPlayer != NULL){
			g_pPlayerDataServer->sendPlayerMsgToGameServer(pRetMsg->sendToPlayerGuid(), pRetMsg);
		}

		return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onCSJoinClan(IMsgHandler* pMsgHandler, Packet* pMsg)
	{
		__ENTER_FUNCTION__
		ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
		CSJoinClan* pJoinMsg = (CSJoinClan*)pMsg;

		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pJoinMsg->playerGuid());
		// 玩家只能在自己的基地中
		Assert(pPlayer != NULL && pPlayer->getGuid() == pJoinMsg->playerGuid());

		//bool bCanDo = pPlayer->canDo(pJoinMsg->GetPacketID());
		bool bCanDo = canDo(pPlayer->gameState(), pJoinMsg->GetPacketID());
		if (!bCanDo){
			CacheLog_ERROR(LOG_ERROR, "player(guid:%d, game state:%d), can not do this in onCSJoinClan()",
				pPlayer->getGuid(), pPlayer->gameState());
			return MSG_HANDLER_SUCCESS;
		}

		bool bInClan = pPlayer->inClan();
		if (bInClan){
			SCJoinClanRet retMsg;
			retMsg.returnValue(RETURN_VALUE_PLAYER_ALREADY_IN_CLAN);
			retMsg.playerGuid(pPlayer->getGuid());
			retMsg.clanGuid(pJoinMsg->clanGuid());

			pClientConnection->SendMsg(&retMsg);
		}
		else{
			pJoinMsg->playerName(pPlayer->name());
			g_pPlayerDataServer->getWorldServerConnector()->SendMsg(pJoinMsg);
		}

		CacheLog_DEBUG(LOG_DEBUG, "onCSJoinClan, player guid:%d, clan guid:%d",
			pJoinMsg->playerGuid(), pJoinMsg->clanGuid());

		return MSG_HANDLER_SUCCESS;

			__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onSCJoinClanRet(IMsgHandler* pMsgHandler, Packet* pMsg)
	{
		__ENTER_FUNCTION__
			// worldserver处理完后发送过来
			SCJoinClanRet* pRetMsg = (SCJoinClanRet*)pMsg;
	
			if (pRetMsg->returnValue() == RETURN_VALUE_NEED_APPROVE_TO_JOIN_CLAN){
				Player* pClanLeader = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pRetMsg->clanLeaderGuid());
				//Assert(pClanLeader != NULL && pClanLeader->clanGuid() == pRetMsg->clanGuid());
				// 会长不在线
				if (pClanLeader == NULL){
					// !! TO DO [1/21/2013 dujiang] 从DB中载入后，加入会长的mailbox
				}
				else{
					MessageMail message;
					message.bIsAddMessage = true;
					message.byType = MAIL_TYPE_JOIN_CLAN_APPLICATION;
					message.nSendTime = CTimeSystem::Instance()->GetBigSecond();
					message.byMessageLen = 0;

					message.nReceiverGuid = pClanLeader->getGuid();
					message.nSenderGuid = pRetMsg->playerGuid();
					//message.bySenderNameLen = strlen(pRetMsg->playerName());
					strncpy(message.szSenderName, pRetMsg->playerName(), PLAYER_NAME_WITH_CLOSING);
					message.szSenderName[PLAYER_NAME_WITH_CLOSING - 1] = 0;
					message.bySenderLevel = pRetMsg->playerLevel();
					pClanLeader->addMail(message);
				}
			}
			else{
				Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pRetMsg->playerGuid());
				Assert(pPlayer != NULL && !pPlayer->inClan());
	
				//if (pRetMsg->returnValue() == RETURN_VALUE_OK){
				//	pPlayer->clanGuid(pRetMsg->clanGuid());
				//	// !! TO DO [1/21/2013 dujiang] need to insert one mail to it's mail with the type of CLAN
				//}
	
				g_pPlayerDataServer->sendPlayerMsgToGameServer(pPlayer->getGuid(), pRetMsg);
			}

			return MSG_HANDLER_SUCCESS;

		__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onCSJoinClanApprove(IMsgHandler* pMsgHandler, Packet* pMsg){
		__ENTER_FUNCTION__
		ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
		CSJoinClanApprove* pJoinApproveMsg = (CSJoinClanApprove*)pMsg;
		Assert(pJoinApproveMsg->clanLeaderGuid() != invalid_guid);

		Player* pClanLeader = 
			PlayerManager::GetInstancePtr()->GetPlayerByGuid(pJoinApproveMsg->clanLeaderGuid());
		Assert(pClanLeader != NULL);

		pClanLeader->removeRequestMail(MAIL_TYPE_JOIN_CLAN_APPLICATION, pJoinApproveMsg->joinPlayerGuid());
		g_pPlayerDataServer->getWorldServerConnector()->SendMsg(pJoinApproveMsg);

		return MSG_HANDLER_SUCCESS;

		__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onSCJoinClanApproveRet( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
		__ENTER_FUNCTION__
			// worldserver处理完后发送过来
			SCJoinClanApproveRet* pRetMsg = (SCJoinClanApproveRet*)pMsg;

		if (pRetMsg->returnValue() == RETURN_VALUE_OK){
			Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pRetMsg->playerGuid());
			//Assert(pClanLeader != NULL && pClanLeader->clanGuid() == pRetMsg->clanGuid());
			// 被批准进入公会的玩家不在线
			if (pPlayer == NULL){
				// !! TO DO [1/21/2013 dujiang] 从DB中载入后,修改其所属clan
			}
			else{
				Assert(pPlayer->clanGuid() == invalid_guid);
				pPlayer->clanGuid(pRetMsg->clanGuid());
			}

			//if (pRetMsg->returnValue() == RETURN_VALUE_OK){
			//	pPlayer->clanGuid(pRetMsg->clanGuid());
			//	// !! TO DO [1/21/2013 dujiang] need to insert one mail to it's mail with the type of CLAN
			//}
			if (pPlayer != NULL){
				g_pPlayerDataServer->sendPlayerMsgToGameServer(pPlayer->getGuid(), pRetMsg);
			}
		}
		return MSG_HANDLER_SUCCESS;
		__LEAVE_FUNCTION__
	}

	extern MSG_HANDLER_RETURN onSCMail(IMsgHandler* pMsgHandler, Packet* pMsg){
__ENTER_FUNCTION__
		SCMail* pMail = (SCMail*)pMsg;
		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pMail->receiverGuid());
		// 目前只用发给在线玩家
		if (pPlayer != NULL && pPlayer->isOnline()){
			g_pPlayerDataServer->sendPlayerMsgToGameServer(pPlayer->getGuid(), pMail);
		}
		return MSG_HANDLER_SUCCESS;

__LEAVE_FUNCTION__
	}

	MSG_HANDLER_RETURN onCSClaimAchievement( IMsgHandler* pMsgHandler, Packet* pMsg )
	{
	__ENTER_FUNCTION__
		ClientConnection* pClientConnection = (ClientConnection*)pMsgHandler;
		CSClaimAchievement* pClaimMsg = (CSClaimAchievement*)pMsg;
		int nAchieveId = pClaimMsg->GetAchieveId();
		Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pClaimMsg->visitorGUID());
		if (pPlayer == NULL)
		{
			return MSG_HANDLER_SUCCESS;
		}
		AchievementItem* pAItem = pPlayer->getAchievementData().GetAchievementItemById(nAchieveId);
		if (pAItem == NULL)
		{
			return MSG_HANDLER_SUCCESS;
		}
		_DBC_T_ACHIEVEMENTS* pACfg = AchievementsConfig::getSingleton()->getCfg(nAchieveId);
		if (pClaimMsg->GetRequestType() == CSClaimAchievement::CA_GetRewards)
		{
			for (int i = 0; i < MAX_ACHIEVEMENT_LEVEL; i++)
			{
				if (pAItem->aClaimedFlags[i] != 1 && pAItem->progress >= pACfg->aCfg[i].nNum)
				{
					pAItem->aClaimedFlags[i] = 1;
					// 给用户经验、代币奖励
					Player *pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pClaimMsg->visitorGUID());
					if (pPlayer != NULL)
					{
						pPlayer->addMoneyNum(pACfg->aCfg[i].nRewardGem);
						pPlayer->addExp(pACfg->aCfg[i].nRewardExp);
					}
					
					SCSendAchievementDataChanged adcMsg;
					adcMsg.visitorGuid(pClaimMsg->visitorGUID());
					adcMsg.SetAchieveId(nAchieveId);
					adcMsg.SetInfoType(SCSendAchievementDataChanged::SADC_AchieveLevelClaimed);
					adcMsg.SetParam1(i);
					pClientConnection->SendMsg(&adcMsg);
				}
			}
		}
		else if (pClaimMsg->GetRequestType() == CSClaimAchievement::CA_UseTitle) // 把称号放到用户信息显示上 
		{
			Player *pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pClaimMsg->visitorGUID());
			if (pPlayer != NULL)
			{
				pPlayer->UseTitleOfAchievementId(nAchieveId);
			}
		}

		return MSG_HANDLER_SUCCESS;

	__LEAVE_FUNCTION__
	}

	//extern MSG_HANDLER_RETURN onSCAskSoldierRet( IMsgHandler* pMsgHandler, Packet* pMsg )
	//{
	//	__ENTER_FUNCTION__
	//		SCAskSoldierRet* pAskMsg = (SCAskSoldierRet*)pMsg;
	//	Player* pPlayer = PlayerManager::GetInstancePtr()->GetPlayerByGuid(pAskMsg->sendToPlayerGuid());
	//	// send the msg if the player is online
	//	if (pPlayer != NULL){
	//		g_pPlayerDataServer->sendPlayerMsgToGameServer(pPlayer->getGuid(), pAskMsg);
	//	}
	//		__LEAVE_FUNCTION__
	//}



}

