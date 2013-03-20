/*
文件	：Main.h
简介	：
作者	：侯志芳
创建日期：2009/12/23

最后修改日期	修改者		修改内容
---------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------
*/


#include <stdio.h>
#include "PlayerDataServer.h"
#include "AssertDefine.h"

PlayerDataServer *g_pPlayerDataServer = NULL;

int main(int argc, char* argv[])
{
	g_pPlayerDataServer = new PlayerDataServer();
	Assert( NULL != g_pPlayerDataServer );

	if ( g_pPlayerDataServer->Init() )
	{
		g_pPlayerDataServer->Start();
	}

	SAFE_DELETE(g_pPlayerDataServer);
	return 0;
}

