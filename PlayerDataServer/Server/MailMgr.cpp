
#include "MailMgr.h"
#include "AssertDefine.h"
#include "PlayerDataServer.h"
#include "AllPacketInclude.h"
#include "Player.h"

MailMgr::MailMgr( void )
{
	__ENTER_FUNCTION__

		__LEAVE_FUNCTION__
}

MailMgr::~MailMgr( void )
{
	__ENTER_FUNCTION__
		m_pOwnerPlayer = NULL;
		__LEAVE_FUNCTION__
}


MailMgr::MailMgr( Player* pOwner )
{
	__ENTER_FUNCTION__
		m_pOwnerPlayer = pOwner;
	__LEAVE_FUNCTION__
}

void MailMgr::add( const MessageMail& mail )
{
	__ENTER_FUNCTION__
		if (mail.byType == MAIL_TYPE_JOIN_CLAN_APPLICATION){
			m_mailRequest.addMail(mail);
			if (m_pOwnerPlayer != NULL && m_pOwnerPlayer->isOnline()){
				SCMail msg;
				msg.messageMail(mail);
				//msg.messageType(MAIL_TYPE_JOIN_CLAN_APPLICATION);
				//msg.receivedTime(mail.nSendTime);
				//msg.messageLen(mail.byMessageLen);
				//msg.message(mail.szMessage);
				//msg.receiverGuid(mail.nReceiverGuid);
				//msg.senderGuid(mail.nSenderGuid);
				//msg.senderNameLen(mail.bySenderNameLen);
				//msg.senderName(mail.szSenderName);
				//msg.senderLevel(mail.bySenderLevel);
				//msg.param(mail.nParam);

				g_pPlayerDataServer->sendPlayerMsgToGameServer(m_pOwnerPlayer->getGuid(), &msg);
			}
		}
		else{
			m_mailMessage.addMail(mail);
		}
	__LEAVE_FUNCTION__
}

void MailMgr::removeRequestMail( unsigned char byType, Guid_t nRequestPlayerGuid )
{
	__ENTER_FUNCTION__
		m_mailRequest.removeRequestMail(byType, nRequestPlayerGuid);
	__LEAVE_FUNCTION__
}

void MailMgr::removeAllRequestMail( unsigned char byType )
{
	__ENTER_FUNCTION__
		m_mailRequest.removeRequestMail(byType);
		__LEAVE_FUNCTION__
}