
#include "LogSystem.h"
#include <stdio.h>
#include "Utility.h"

CLogSystem* CLogSystem::m_pInstance = NULL;

CLogSystem::CLogSystem(): 
    m_eFileLogLevel(LOG_LEVEL_DEBUG),
	m_eConsoleLogLevel(LOG_LEVEL_DEBUG)
{
    m_bFileNameAddPostfix = TRUE;

	for (INT i = 0; i < MAX_LOG_FILE_NUM; ++i)
	{
		m_pLogFiles[i] = NULL;
	}
}

CLogSystem::~CLogSystem()
{
	if (m_pDefaultFile != NULL)
	{
		delete m_pDefaultFile;
		m_pDefaultFile = NULL;
	}
}

CLogSystem* CLogSystem::Instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CLogSystem();
	}
	return m_pInstance;
}

bool CLogSystem::InitLogFile(INT logFileId, const CHAR* szFileName, const CHAR* szLogPath )
{
	if (logFileId < 0 || logFileId >= MAX_LOG_FILE_NUM )
	{
		DiskLog_ERROR(LOG_ASSERT, "CLogSystem InitLogFile faild! Wrong FileId : %d . file name : %s",logFileId , szFileName);
		return FALSE;
	}
	if (m_pLogFiles[logFileId] != NULL)
	{
		DiskLog_ERROR(LOG_ASSERT, "CLogSystem InitLogFile faild! FileId has been used. FileId : %d . file name : %s",logFileId , szFileName);
		return FALSE;
	}
	m_pLogFiles[logFileId] = new CLogFile(szFileName, szLogPath);
	m_pLogFiles[logFileId]->SetConsoleLogLevel(m_eConsoleLogLevel);
	m_pLogFiles[logFileId]->SetFileLogLevel(m_eFileLogLevel);
	return TRUE;
}

VOID CLogSystem::Exit()
{
	for (INT i = 0; i < MAX_LOG_FILE_NUM; i++)
	{
		if (m_pLogFiles[i] != NULL)
		{
			delete m_pLogFiles[i];
			m_pLogFiles[i] = NULL;
		}
	}
	if (m_pDefaultFile != NULL)
	{
		delete m_pDefaultFile;
		m_pDefaultFile = NULL;
	}
}

CLogFile* CLogSystem::GetLogFlieById(INT logFileId)
{
	if (logFileId < 0 || logFileId >= MAX_LOG_FILE_NUM )
	{
		return m_pDefaultFile;
	}
	if(m_pLogFiles[logFileId] == NULL)
	{
		return m_pDefaultFile;
	}
	return m_pLogFiles[logFileId];
}

VOID CLogSystem::Flush()
{
	for (INT i = 0; i < MAX_LOG_FILE_NUM; ++i)
	{
		if(m_pLogFiles[i] != NULL)
		{
			m_pLogFiles[i]->WriteAllBufToFile();
		}
	}
}

bool CLogSystem::Initialize( bool bCreateLogDir /*= TRUE*/, bool bLogFilenameAddPostfix /*= TRUE*/ )
{
    if (bCreateLogDir)
    {
        if (!CheckDirExsist("./Log"))
        {
            DSCreateDirectory("./Log");
        }
    }
    m_bFileNameAddPostfix = bLogFilenameAddPostfix;
    //初始化默认日志文件
    m_pDefaultFile = new CLogFile("Default.log", "./Log");
    m_pDefaultFile->SetConsoleLogLevel(m_eConsoleLogLevel);
    m_pDefaultFile->SetFileLogLevel(m_eFileLogLevel);

    return TRUE;
}