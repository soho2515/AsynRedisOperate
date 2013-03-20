#include "PlayerDataServer.h"
#include "PacketFactoryManager.h"
#include "AllPacketInclude.h"
#include "MsgHandler.h"

VOID PlayerDataServer::registerAllPacketFactory()
{
	PacketFactoryManager::GetInstancePtr()->Init(MSGID_MAX_SIZE);
	//=============================================================
	// dujiang整理过的正式流程的消息


	// game server --> player data server
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<HeartBeatMsg>, (pPacketHandlerFunc)MSG_HANDLER::onHeartBeat);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSAskPlayerInfo>, (pPacketHandlerFunc)MSG_HANDLER::onAskPlayerInfo);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSAskEnterPersonalBase>, (pPacketHandlerFunc)MSG_HANDLER::onAskEnterPersonalBase);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<GDUnregVisitor>, (pPacketHandlerFunc)MSG_HANDLER::onGDUnregVisitor);


	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSCreateBuilding>, (pPacketHandlerFunc)MSG_HANDLER::onCSCreateBuilding);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSBuildingOperation>, (pPacketHandlerFunc)MSG_HANDLER::onCSBuildingOperation);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSPublicHouseOperate>, (pPacketHandlerFunc)MSG_HANDLER::onCSPublicHouseOperate);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSUpgradeOperate>, (pPacketHandlerFunc)MSG_HANDLER::onCSUpgradeOperate);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSCreateClan>, (pPacketHandlerFunc)MSG_HANDLER::onCSCreateClan);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSJoinClan>, (pPacketHandlerFunc)MSG_HANDLER::onCSJoinClan);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSJoinClanApprove>, (pPacketHandlerFunc)MSG_HANDLER::onCSJoinClanApprove);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<CSClaimAchievement>, (pPacketHandlerFunc)MSG_HANDLER::onCSClaimAchievement);

	// world server --> player data server
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<SCCreateClanRet>, (pPacketHandlerFunc)MSG_HANDLER::onSCCreateClanRet);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<SCLeaveClanRet>, (pPacketHandlerFunc)MSG_HANDLER::onSCLeaveClanRet);

	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<SCKickoutClanMemberRet>, (pPacketHandlerFunc)MSG_HANDLER::onSCKickoutClanMemberRet);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<SCPromoteClanMemberRet>, (pPacketHandlerFunc)MSG_HANDLER::onSCPromoteClanMemberRet);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<SCJoinClanRet>, (pPacketHandlerFunc)MSG_HANDLER::onSCJoinClanRet);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<SCDemoteClanMemberRet>, (pPacketHandlerFunc)MSG_HANDLER::onSCDemoteClanMemberRet);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<SCJoinClanApproveRet>, (pPacketHandlerFunc)MSG_HANDLER::onSCJoinClanApproveRet);
	REGISTER_PACKETFACTORY_WITH_HANDLER(new PacketFactory_T<SCMail>, (pPacketHandlerFunc)MSG_HANDLER::onSCMail);

	// player data server --> game 
	REGISTER_PACKETFACTORY_WITHOUT_HANDLER(new PacketFactory_T<SCSendPlayerInfo>);
	REGISTER_PACKETFACTORY_WITHOUT_HANDLER(new PacketFactory_T<SCSendPersonalBaseInfo>);

	REGISTER_PACKETFACTORY_WITHOUT_HANDLER(new PacketFactory_T<SCCreateBuilding>);
	REGISTER_PACKETFACTORY_WITHOUT_HANDLER(new PacketFactory_T<SCSendBuildInfo>);

}
