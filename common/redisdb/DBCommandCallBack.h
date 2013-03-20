/********************************************************************
	created:	2013/02/19
	created:	19:2:2013   15:23
	filename: 	c:\work\code\Server\common\redisdb\DBCommandCallBack.h
	file path:	c:\work\code\Server\common\redisdb
	file base:	DBCommandCallBack
	file ext:	h
	author:		houzhifang
	
	purpose:	
*********************************************************************/

#ifndef __DBCommandCallBack_h__
#define __DBCommandCallBack_h__
#pragma once

#include "ServerDefine.h"

enum DBCOMMAND_CALLBACK_TYPE{
	DBCOMMAND_CALLBACK_ASKPLAYERINFO = 0,
	DBCOMMAND_CALLBACK_LOGIN,

	DBCOMMAND_CALLBACK_SIZE,
};

//状态是让工厂用的，用来区分命令的状态，从而进行复用。
enum DBCOMMAND_CALLBACK_STATUS
{
	DBCOMMAND_CALLBACK_STATUS_IDLE,
	DBCOMMAND_CALLBACK_STATUS_INIT,
	DBCOMMAND_CALLBACK_STATUS_OPERATE,
};

class DBCommandCallBack
{
public:
	DBCommandCallBack():m_bSignal(false),m_Status(DBCOMMAND_CALLBACK_STATUS_IDLE){}
	virtual ~DBCommandCallBack(){};
	virtual DBCOMMAND_CALLBACK_TYPE getType() = 0;
	virtual DBCOMMAND_CALLBACK_STATUS GetStatus() const {
		return m_Status;
	}
	virtual void SetStatus(DBCOMMAND_CALLBACK_STATUS status){
		m_Status = status;
	}
	void Signal();
	bool canExcute();
	virtual void CleanUp();
	virtual void ExcuteCallBack() = 0;

protected:
		CLock m_datalock;

private:
	CLock m_lock;
	bool m_bSignal;
	DBCOMMAND_CALLBACK_STATUS m_Status;
};

#endif // __DBCommandCallBack_h__
