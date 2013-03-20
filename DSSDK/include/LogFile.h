#ifndef	_LOG_FILE_H_
#define	_LOG_FILE_H_

#include "BaseType.h"
#include "lock.h"

#define MAX_FILE_NAME	256
#define MAX_LOG_LINE	8192
#define LOG_BUF_LEN		1024*1024

enum enum_LOG_LEVEL
{
    LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_WARNNING,
	LOG_LEVEL_ERROR,
};

class CLogFile
{
public:
	CLogFile(const CHAR* szFileName, const CHAR* szPath );
	~CLogFile();

public:
	//日志写到内存缓冲区，等缓冲满时，或一定的时间间隔时，统一写入文件
	VOID CacheLog(enum_LOG_LEVEL logLevel, const CHAR* pLogString , ...);

	//直接写入文件中，若缓冲区有未写入
	VOID DiskLog(enum_LOG_LEVEL	logLevel, const CHAR* pLogString , ...);

	//检测back缓冲区，有内容就写入文件
	VOID WriteBackBufToFile();

	//front 和 back 缓冲区，都写入文件
	VOID WriteAllBufToFile();

	//检查文件大小，如果需要的话，就新建一个文件
	VOID CheckLogFileStat();

	VOID SetFileLogLevel(enum_LOG_LEVEL	logLevel) { m_eFileLogLevel = logLevel; }
	VOID SetConsoleLogLevel(enum_LOG_LEVEL logLevel) { m_eConsoleLogLevel = logLevel; }
private:
	VOID SwitchBuf();
    VOID MakeLogfilename();
    VOID MakeLogfilenameWithPostfix();
private:
	enum_LOG_LEVEL		m_eFileLogLevel;
	enum_LOG_LEVEL		m_eConsoleLogLevel;

	CHAR		m_szLogFileName[MAX_FILE_NAME];
	CHAR		m_szFileName[MAX_FILE_NAME];
	CHAR        m_szFilePath[MAX_FILE_NAME];
	CHAR		m_pBuf[2*LOG_BUF_LEN];
	CHAR*		m_pFrontBuf;
	CHAR*		m_pBackBuf;
	CHAR*		m_pWriteBuf;
	
	bool		m_bBackBufAvailable;
	CLock		m_lock;

	//日志文件超过这个大小，就新开一个文件
	INT			m_nMaxLogFileLen;
	//记录当前的日期，如果发现跨过一天，就新开一个文件
	INT			m_nDate;
};

#endif
