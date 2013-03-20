/********************************************************************
	created:	2013/02/21
	created:	21:2:2013   12:28
	filename: 	c:\work\code\Server\PlayerDataServer\Server\DBCommand_Personalbase.h
	file path:	c:\work\code\Server\PlayerDataServer\Server
	file base:	DBCommand_Personalbase
	file ext:	h
	author:		houzhifang
	
	purpose:	
*********************************************************************/

#ifndef __DBCommand_Personalbase_h__
#define __DBCommand_Personalbase_h__
#pragma once


#include "ServerDefine.h"
#include "redisdb/DBCommand.h"
#include "redisdb/DBCommandFactory.h"

/*
class DBCommand_Personal_Load : public DBCommand
{
public:
	DBCommand_Personal_Load():m_nGuid(invalid_guid){
	};
	virtual ~DBCommand_Personal_Load(){};

	virtual DBCOMMAND_TYPE GetType(){
		return DBCOMMAND_TYPE_PERSONAL_LOAD;
	}
	virtual void CleanUp(){
		DBCommand::CleanUp();
	}

	bool InitDBCommand(const Guid_t nGuid);
	virtual bool Execute();

protected:
	INT  m_nGuid;;
};
*/

class DBCommand_Personal_Save : public DBCommand
{
public:
	DBCommand_Personal_Save(){
		m_PersonalInfo.clear();
	};
	virtual ~DBCommand_Personal_Save(){};

	virtual DBCOMMAND_TYPE GetType(){
		return DBCOMMAND_TYPE_PERSONAL_SAVE;
	}
	virtual void CleanUp(){
		DBCommand::CleanUp();
	}

	bool InitDBCommand(const PersonalBaseInfo& personalinfo);
	virtual bool Execute();

private:
	PersonalBaseInfo m_PersonalInfo;
};


#endif // __DBCommand_Personalbase_h__
