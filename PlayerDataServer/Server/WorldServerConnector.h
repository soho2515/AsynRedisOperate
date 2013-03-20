/**********************************************************************
	文件:		WorldServerConnector.h
	简介	
	作者:		dujiang
	创建日期:	2012/11/20   15:09

	版权所有(C) 2012

	最后修改日期	修改者		修改内容
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/


#ifndef WorldServerConnector_h__
#define WorldServerConnector_h__

#pragma once

#include "ServerConnection.h"

class WorldServerConnector :
	public ServerConnection
{
public:
	WorldServerConnector(void);
	virtual ~WorldServerConnector(void);

protected:
	// 连通server后的处理
	virtual	void	onConnected();
};

#endif // WorldServerConnector_h__

