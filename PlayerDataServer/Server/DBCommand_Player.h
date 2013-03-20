/********************************************************************
	created:	2013/02/18
	created:	18:2:2013   16:51
	filename: 	c:\work\code\Server\PlayerDataServer\Server\DBCommand_Player.h
	file path:	c:\work\code\Server\PlayerDataServer\Server
	file base:	DBCommand_Player
	file ext:	h
	author:		houzhifang
	
	purpose:	
*********************************************************************/

#ifndef __DBCOMMAND_PLAYER_H__
#define __DBCOMMAND_PLAYER_H__

#include "ServerDefine.h"
#include "redisdb/DBCommand.h"
#include "redisdb/DBCommandFactory.h"

class DBCommand_Player_Load : public DBCommand
{
public:
	DBCommand_Player_Load():m_nGuid(-1){
		this->CleanUp();
	};
	virtual ~DBCommand_Player_Load(){};

	virtual DBCOMMAND_TYPE GetType(){
		return DBCOMMAND_TYPE_PLAYER_LOAD;
	}
	virtual void CleanUp(){
		DBCommand::CleanUp();
		m_nGuid = -1;
	}

	bool InitDBCommand(const int nGuid);
	virtual bool Execute();

private:
	int  m_nGuid;;
};

class DBCommand_Player_Save : public DBCommand
{
public:
	DBCommand_Player_Save():m_nChangeFlag(0){
		this->CleanUp();
	};
	virtual ~DBCommand_Player_Save(){};

	virtual DBCOMMAND_TYPE GetType(){
		return DBCOMMAND_TYPE_PLAYER_SAVE;
	}
	virtual void CleanUp(){
		DBCommand::CleanUp();
		m_PlayerInfo.clear();
		m_nChangeFlag = 0;
	}

	bool InitDBCommand(const PlayerInfo& playerinfo, const int nChangeFlag);
	virtual bool Execute();

private:
	PlayerInfo m_PlayerInfo;
	int m_nChangeFlag;
};


#endif  //__DBCOMMAND_PLAYER_H__
