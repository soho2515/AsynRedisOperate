

/**********************************************************************
	文件:		ClientConnOnPlayerDataSrv.h
	简介	
	作者:		dujiang
	创建日期:	2012/12/10   15:17

	版权所有(C) 2012

	最后修改日期	修改者		修改内容
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/
#ifndef ClientConnOnPlayerDataSrv_h__
#define ClientConnOnPlayerDataSrv_h__

#pragma once

#include "ClientConnection.h"

class ClientConnOnPlayerDataSrv :
	public ClientConnection
{
public:
	ClientConnOnPlayerDataSrv(void);
	virtual ~ClientConnOnPlayerDataSrv(void);

	virtual void onClose();
};

#endif // ClientConnOnPlayerDataSrv_h__

