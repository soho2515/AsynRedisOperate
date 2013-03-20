/**********************************************************************
	文件:		ClientConnOnPlayerDataSrvMgr.h
	简介	
	作者:		dujiang
	创建日期:	2012/12/10   15:18

	版权所有(C) 2012

	最后修改日期	修改者		修改内容
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/

#ifndef ClientConnOnPlayerDataSrvMgr_h__
#define ClientConnOnPlayerDataSrvMgr_h__

#pragma once
#include "ClientConnectionManager.h"
#include "ClientConnOnPlayerDataSrv.h"

class ClientConnOnPlayerDataSrvMgr :
	public ClientConnectionManager
{
public:
	ClientConnOnPlayerDataSrvMgr(void);
	virtual ~ClientConnOnPlayerDataSrvMgr(void);

	virtual ClientConnection* createClientConnection(){
		return new ClientConnOnPlayerDataSrv();
	}

};


#endif // ClientConnOnPlayerDataSrvMgr_h__
