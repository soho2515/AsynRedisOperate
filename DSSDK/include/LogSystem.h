#ifndef	_LOG_SYSTEM_H_
#define	_LOG_SYSTEM_H_

#include "BaseType.h"
#include "LogFile.h"

#define MAX_LOG_FILE_NUM 16

enum enum_LOG_FILE_ID
{
	LOG_DEBUG,
	LOG_ASSERT,
	LOG_ERROR,
	LOG_FUNCTION,
	LOG_SEND,
	LOG_LUA_ERROR,
    LOG_DSSDK,
	LOG_MONEY,
	LOG_ITEM,
	LOG_AUDIT,
};


class CLogSystem
{
public:
	CLogSystem();
	~CLogSystem();

public:
	static CLogSystem* Instance();
private:
	static CLogSystem*	m_pInstance;

public:
    // 初始化日志系统,指定是否创建单独的log目录、是否log文件名添加时间、日期等的后缀
    bool Initialize(bool bCreateLogDir = TRUE, bool bLogFilenameAddPostfix = TRUE);

	//初始化一个日志文件，指定一个文件id，打日志时提供这个文件id即打入此文件
	bool InitLogFile(INT logFileId, const CHAR* szFileName, const CHAR* szLogPath );

	//根据文件id查找日志文件
	CLogFile* GetLogFlieById(INT logFileId);

	//设置日志文件的日志等级，高等与这个等级的日志才会被打入文件
	VOID SetFileLogLevel(enum_LOG_LEVEL	logLevel) { m_eFileLogLevel = logLevel; }

	//设置控制台输出的日志等级，高等与这个等级的日志才会被打入控制台
	VOID SetConsoleLogLevel(enum_LOG_LEVEL logLevel) { m_eConsoleLogLevel = logLevel; }

    bool GetFileNameAddPostfix() { return m_bFileNameAddPostfix; }

	//刷新所有日志的backbuf到文件
	VOID Flush();

	VOID Exit();

private:
	enum_LOG_LEVEL		m_eFileLogLevel;
	enum_LOG_LEVEL		m_eConsoleLogLevel;
    bool                m_bFileNameAddPostfix; //日志文件名是否增加后缀
    CLogFile*			m_pLogFiles[MAX_LOG_FILE_NUM];

	//默认日志文件，当指定的文件id有错，找不到对应文件时，打入此文件
	CLogFile*			m_pDefaultFile;

};

#define DiskLog_WARNNING(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->DiskLog(LOG_LEVEL_WARNNING, format, ##__VA_ARGS__)
#define DiskLog_DEBUG(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->DiskLog(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define DiskLog_ERROR(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->DiskLog(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#define CacheLog_WARNNING(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->CacheLog(LOG_LEVEL_WARNNING, format, ##__VA_ARGS__)
#define CacheLog_DEBUG(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->CacheLog(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define CacheLog_ERROR(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->CacheLog(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#endif
