/**********************************************************************
	文件:		MsgHandler.h
	简介	
	作者:		dujiang
	创建日期:	2012/11/16   18:10

	版权所有(C) 2012

	最后修改日期	修改者		修改内容
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/


#ifndef MsgHandler_h__
#define MsgHandler_h__

#pragma once

#include "IMsgHandler.h"
#include "Packet.h"

namespace MSG_HANDLER{
	extern MSG_HANDLER_RETURN onHeartBeat(IMsgHandler* pMsgHandler, Packet* pMsg);
	


	//=============================================================
	// dujiang整理过的正式流程的消息

	// game server --> player data server
	extern MSG_HANDLER_RETURN onAskPlayerInfo(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onAskEnterPersonalBase(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onGDUnregVisitor(IMsgHandler* pMsgHandler, Packet* pMsg);

	extern MSG_HANDLER_RETURN onCSCreateBuilding(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onCSBuildingOperation(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onCSPublicHouseOperate(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onCSUpgradeOperate(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onCSCreateClan(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onCSJoinClan(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onCSJoinClanApprove(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onCSClaimAchievement(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onCSGiveSoldier(IMsgHandler* pMsgHandler, Packet* pMsg);

	// world server --> player data server
	extern MSG_HANDLER_RETURN onSCCreateClanRet(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onSCLeaveClanRet(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onSCKickoutClanMemberRet(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onSCPromoteClanMemberRet(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onSCDemoteClanMemberRet(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onSCJoinClanRet(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onSCJoinClanApproveRet(IMsgHandler* pMsgHandler, Packet* pMsg);
	extern MSG_HANDLER_RETURN onSCMail(IMsgHandler* pMsgHandler, Packet* pMsg);
};



#endif // MsgHandler_h__
