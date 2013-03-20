/**********************************************************************
	文件:		MailMgr.h
	简介	
	作者:		dujiang
	创建日期:	2013/1/21   11:01

	版权所有(C) 2012

	最后修改日期	修改者		修改内容
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/

#ifndef MailMgr_h__
#define MailMgr_h__

#pragma once

#include "MailBox.h"
#include "DataDefine.h"

class Player;

class MailMgr
{
public:
	//
	MailMgr(Player* pOwner);
	~MailMgr(void);


	void	add(const MessageMail& mail);
	void	removeRequestMail(	unsigned char byType, Guid_t nRequestPlayerGuid);
	void	removeAllRequestMail( unsigned char byType );
protected:
	MailMgr(void);


private:
	Player*		m_pOwnerPlayer;

	// 最近的消息
	MailBox<MessageMail, MAX_MESSAGE_MAIL_IN_MAILBOX_COUNT>	m_mailMessage;
	// 等待玩家处理的消息
	MailBox<MessageMail, MAX_MESSAGE_MAIL_IN_MAILBOX_COUNT> m_mailRequest;

};

#endif // MailMgr_h__

