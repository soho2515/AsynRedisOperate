/*
	文件	：DBCommand.h
	简介	：DBCommand基类，定义处理DB命令的基础
	          对于DBCommand，大致先分为两类，一类是CHAR相关的，另一类时manager类型的（例如商店管理器等）
			  对于CHAR相关的数据，其特殊在于只会被LoginServer加载，SceneServer只会保存，而且每次保存，为了降低通信量，可以是SMUnit的其中一部分。
			  对于manager类型的，其在SceneServer的Sharemem启动时（大部分时起服时）就需要加载，之后只会保存，不会再次加载。
			  所以，大致规划Command的子类：Char_Load单独处理从loginserver来的加载角色命令，其中又分为读取账户下的角色信息和读取某条角色整体消息，
			  Char_Create单独处理从LoginServer来的创建角色命令， Char_Save_Table单独处理SceneServer发起的存储角色某个表的的命令，Unit_Load,Unit_Save
			  分别处理manager相关的命令处理，其中save需要调用存储过程，以使存在则update，不存在则insert的逻辑。
	作者	：侯志芳
	创建日期：2010/01/04

	版权所有(C) 2009-2010 竞秀天下（北京）科技有限公司

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DB_COMMAND_H__
#define __DB_COMMAND_H__

#include "BaseType.h"

enum DBCOMMAND_TYPE
{
	DBCOMMAND_TYPE_PLAYER_LOAD = 0,                 // for playergameserver
	DBCOMMAND_TYPE_PLAYER_SAVE,                         // for playergameserver
	DBCOMMAND_TYPE_PERSONAL_LOAD,                  // for playergameserver
	DBCOMMAND_TYPE_PERSONAL_SAVE,                // for playergameserver

	DBCOMMAND_TYPE_PLAYER_LOGIN,
	DBCOMMAND_TYPE_PLAYER_REGISTER,

	DBCOMMAND_TYPE_SIZE,
};

enum DBCOMMAND_PRIORITY
{
	DBCOMMAND_PRIORITY_LEVEL_HIGH = 0,
	DBCOMMAND_PRIORITY_LEVEL_MID,
	DBCOMMAND_PRIORITY_LEVEL_LOW,
};

//状态是让工厂用的，用来区分命令的状态，从而进行复用。
enum DBCOMMAND_STATUS
{
	DBCOMMAND_STATUS_IDLE,
	DBCOMMAND_STATUS_INIT,
	DBCOMMAND_STATUS_OPERATE,
};

#define DEFAULT_BLANK_CONTENT   "~@blank@!"

class IMsgHandler;
class DBCommand
{
public:
	DBCommand()
	:m_Priority(DBCOMMAND_PRIORITY_LEVEL_MID),m_Status(DBCOMMAND_STATUS_IDLE),m_pMsgHandler(NULL),m_nDBCallbackID(-1){

	};

	virtual ~DBCommand(){};

//	virtual BOOL BuildCommand() = 0;
	virtual bool Execute() = 0;
	virtual void CleanUp(){ m_nDBCallbackID = -1; };       //由于DBCommand是复有的，所以必须提供CleanUp函数，以供复用时清空调用。
	virtual DBCOMMAND_TYPE GetType() = 0;
	virtual DBCOMMAND_PRIORITY  GetPriority() const {
		return m_Priority;
	}
	virtual void SetPriority(DBCOMMAND_PRIORITY priority){
		m_Priority = priority;
	}
	virtual DBCOMMAND_STATUS GetStatus() const {
		return m_Status;
	}
	virtual void SetStatus(DBCOMMAND_STATUS status){
		m_Status = status;
	}

	virtual unsigned int GetSaveSerail() const {
		return m_nSaveSerail;
	}

	void setCallbackID(const unsigned int nID){
		m_nDBCallbackID = nID; 
	}

	int getCallbackID() const{
		return m_nDBCallbackID;
	}

	//下方的m_pMsgHandler用于在Execute中进行处理消息。
	IMsgHandler* GetMsgHandler() const { return m_pMsgHandler; }
	VOID SetMsgHandler(IMsgHandler* nMsgHandler) { m_pMsgHandler = nMsgHandler; }

protected:
	DBCOMMAND_PRIORITY m_Priority;
	DBCOMMAND_STATUS m_Status;
	IMsgHandler*  m_pMsgHandler;
	unsigned int  m_nSaveSerail;
	int m_nDBCallbackID;
};
#endif
