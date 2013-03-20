
#ifndef PlayerDataServer_h__
#define PlayerDataServer_h__

#pragma once

#include "BaseType.h"
#include "ServerBase.h"

class PlayerDataServer : public CServerBase
{
public:
	PlayerDataServer(void);
	virtual ~PlayerDataServer(void);

	virtual VOID	DoStart();
	virtual VOID	DoTick();
	virtual bool	DoInitGameData();
	virtual VOID	OnExit();

private:
	VOID			registerAllPacketFactory();
	bool RegisterAllDBCommand();
	bool RegisterAllDBCommandCallBack();
};

extern PlayerDataServer *g_pPlayerDataServer;

#endif // PlayerDataServer_h__

