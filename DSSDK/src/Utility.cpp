#include "Utility.h"

#include <signal.h>
#include <stdio.h>
#include <string.h>

#ifndef __LINUX
#include <malloc.h>
#endif

#include <stdlib.h>
#include <ctype.h> 
#include <stdarg.h>
#include <limits>

#include "TimeSystem.h"
#include "IThread.h"

#if defined(__LINUX)
# include <pthread.h>
//# include <execinfo.h>
# include <unistd.h>
# include <sys/stat.h>
#endif



static SigHandleCallbackFun g_pAppExitFun = NULL;
static char g_strDumpFileName[128] = "Nodefine"; 
static LPDSSDK_LOGFUNCTION s_pSdkLogFun = NULL;

static void signalHandler(int nSigId)
{
//	FILE *dumpfile;
//	dumpfile = fopen(g_strDumpFileName, "a+"); //dump日志采用追加方式，防止覆盖掉以前的还未追查的崩溃
//
//	DateTime now;
//    fprintf(dumpfile, "========== %.4d-%.2d-%.2d %.2d:%.2d:%.2d begin =========== \n", 
//        now.GetYear(), now.GetMonth(), 
//        now.GetDay(), now.GetHour(), 
//        now.GetMinute(), now.GetSecond());
//
//    const CHAR * szSigName = "Other";
//
//    switch(nSigId)
//    {
//    case SIGINT:
//        szSigName = "SIGINT";
//        break;
//    case SIGILL:
//        szSigName = "SIGILL";
//        break;
//    case SIGFPE:
//        szSigName = "SIGFPE";
//        break;
//    case SIGSEGV:
//        szSigName = "SIGSEGV";
//        break;
//    case SIGTERM:
//        szSigName = "SIGTERM";
//        break;
//    case SIGABRT:
//        szSigName = "SIGABRT";
//        break;
//#ifdef __LINUX
//    case SIGXFSZ:
//        szSigName = "SIGXFSZ";
//        break;
//#endif
//    default:
//        break;
//    }
//
//	fprintf(dumpfile, "Application Error! signal:%d [%s]\n", nSigId, szSigName);
//
//	// 打印堆栈
//	std::string stackString = GetStackString();
//	fprintf( dumpfile, "%s", stackString.c_str() );
//
//	fclose(dumpfile);
//	if (g_pAppExitFun)
//	{
//		(*g_pAppExitFun)();
//	}
//	// 退出线程
//#ifdef __LINUX
//    IThread::__DecreaseCount();
//	pthread_exit(NULL);
//#endif
}

void registSignalHandler(const char* szDumpFileName,  SigHandleCallbackFun pAppExitFun)
{
	strncpy(g_strDumpFileName, szDumpFileName, sizeof(g_strDumpFileName));
	g_pAppExitFun = pAppExitFun;
#ifdef __LINUX
	signal(SIGPIPE,	 SIG_IGN);
	signal(SIGXFSZ,	 signalHandler);
#endif
	signal(SIGINT,	 signalHandler);
	signal(SIGILL,	 signalHandler);
	signal(SIGFPE,	 signalHandler);
	signal(SIGSEGV,  signalHandler);
	signal(SIGTERM,  signalHandler);
	//signal(SIGBREAK, signalHandler);
	signal(SIGABRT,	 signalHandler);
}


void ParseFilePath(char* szPath)
{
	if (szPath == NULL)
	{
		return;
	}

	char* pStart = szPath;
	while (*pStart)
	{
		if (*pStart == '\\')
		{
			*pStart = '/';
		}
		++pStart;
	}

	int nLen = strlen(szPath);
	for (int i = nLen; i >= 0; i--)
	{
		if (szPath[i] == '/')
		{
			szPath[i] = '\0';
		}
		else
		{
			break;
		}
	}
	return;
}

void GetWorkingDir(const int nBuffSize, char* szBuff)
{
#if defined(__LINUX)
	getcwd(szBuff, nBuffSize);
#elif defined(__WINDOWS)
	GetCurrentDirectory(nBuffSize, szBuff);
#endif
}

bool CheckDirExsist(const CHAR* szPath)
{
	if (szPath == NULL)
	{
		return FALSE;
	}
	INT nRet = 0;
#if defined(__LINUX)
	nRet = access(szPath, F_OK);
#elif defined(__WINDOWS)
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szPath, &wfd);
	if (hFind != INVALID_HANDLE_VALUE && wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
	{
		nRet = 0;
	}
	else
	{
		nRet = 1;
	}
#endif
	return nRet == 0 ? TRUE : FALSE;
}

bool CheckFileExsist(const CHAR* szPath)
{
	if (szPath == NULL)
	{
		return FALSE;
	}
	INT nRet = 0;
#if defined(__LINUX)
	nRet = access(szPath, F_OK);
#elif defined(__WINDOWS)
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szPath, &wfd);
	if (hFind != INVALID_HANDLE_VALUE && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		nRet = 0;
	}
	else
	{
		nRet = 1;
	}
#endif
	return nRet == 0 ? TRUE : FALSE;
}

VOID DSDeleteFile(const CHAR* szPath)
{
	if (szPath == NULL)
	{
		return;
	}
#if defined(__LINUX)
	remove(szPath);
#elif defined(__WINDOWS)
	DeleteFile(szPath);
#endif
}

bool DSCreateDirectory(const CHAR* szPath)
{
	bool bRet = TRUE;
#if defined(__LINUX)
	if (mkdir(szPath, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
	{
		bRet = FALSE;
	}
#elif defined(__WINDOWS)
	bRet = (bool)CreateDirectory(szPath, NULL);
	if (!bRet && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		bRet = TRUE;
	}
#endif
	return bRet;
}

unsigned long GetThreadID()
{
	unsigned long nRet = 0;
#if defined(__LINUX)
	pthread_t tid = pthread_self();
	nRet = (unsigned long) tid;
#elif defined(__WINDOWS)
	unsigned long ulThreadID = GetCurrentThreadId();
	nRet = ulThreadID;
#endif
	return nRet;
}

//由于linux上的RAND_MAX太大了，是int的最大值，所以在下面的计算时会计算出负数，所以定义自己的rand值为windows上的大小。
#define S8_RAND_MAX  0x7fff

// [start, end)
INT RangeRandom(INT start, INT end)
{
	INT nRandRet = rand() & S8_RAND_MAX;
	double r = (double)(nRandRet);
	return INT(r/(S8_RAND_MAX + 1)*(end - start) + start);
}

FLOAT RangeRandom(FLOAT start, FLOAT end)
{
	INT nRandRet = rand() & S8_RAND_MAX;
	DOUBLE r = (DOUBLE)nRandRet;
	return(FLOAT)(r/(S8_RAND_MAX + 1)*(end - start) + start);
}

// fix me : 把 new 和 delete 去掉
// 算法思想：首先从n中随机选取一个，接着在n-1个剩下的元素中选取下一个，以此类推
VOID GetP( INT m, INT n, INT* array )
{
	if (m > n)
	{
		return;
	}
	if ( NULL == array )
	{
		return;
	}

	bool* isSelectArray = new bool[n];
	memset( isSelectArray, 0, sizeof(bool)*n );
	for ( INT i=0; i<m; ++i )
	{
		INT x = rand() % (n-i); 
		INT selected = 0;
		for( INT j=-1; TRUE; ++selected)
		{
			if ( isSelectArray[selected] == FALSE )
			{
				++j;
			}

			if ( j == x )
			{
				break;
			}
		}
		array[i] = selected;
		isSelectArray[selected] = TRUE;
	}
	delete [] isSelectArray;
}

BYTE  HexToByte(CHAR cChar)
{
	BYTE byte;
	switch(toupper(cChar))
	{
	case '0':
		byte  = 0;
		break;
	case '1':
		byte  = 1;
		break;
	case '2':
		byte  = 2;
		break;
	case '3':
		byte  = 3;
		break;
	case '4':
		byte  = 4;
		break;
	case '5':
		byte  = 5;
		break;
	case '6':
		byte  = 6;
		break;
	case '7':
		byte  = 7;
		break;
	case '8':
		byte  = 8;
		break;
	case '9':
		byte  = 9;
		break;
	case 'A':
		byte  = 10;
		break;
	case 'B':
		byte  = 11;
		break;
	case 'C':
		byte = 12;
		break;
	case 'D':
		byte = 13;
		break;
	case 'E':
		byte = 14;
		break;
	case 'F':
		byte = 15;
		break;
	default:
		byte = 0;
		break;
	}
	return byte;
}

bool BinaryToString(const VOID* pBuf, const UINT nBufLen, CHAR* pszBuf, const UINT nSzBuffLen)
{
	if (nSzBuffLen < nBufLen*2 || nSzBuffLen <= 0)
	{
		return FALSE;
	}

	CHAR szTemp[3] = {0};
	pszBuf[0] = '\0';
	for (UINT i = 0; i < nBufLen; i++)
	{
		sprintf(szTemp, "%.2x", ((BYTE*)pBuf)[i]);
		strncat(pszBuf, szTemp, 2);
	}

	return TRUE;
}

bool  StringToBinary(const CHAR* pszBuf, VOID* pBuf, UINT nLen)
{
	return StringToBinary( pszBuf, pszBuf+strlen(pszBuf), pBuf, nLen );
}

bool  StringToBinary(const CHAR* pszBufBegin, const CHAR* pszBufEnd, VOID* pBuf, UINT nLen)
{
	UINT nSzLen = pszBufEnd - pszBufBegin;
	if (nSzLen%2 != 0 || nSzLen < nLen*2)
	{
		return FALSE;
	}

	INT nBufLen = 0;
	for (UINT i = 0; i < nSzLen; )
	{
		BYTE high = HexToByte(pszBufBegin[i++]);
		BYTE low = HexToByte(pszBufBegin[i++]);
		((BYTE*)pBuf)[nBufLen++] = high*16+low;
	}

	return TRUE;
}

LPDSSDK_LOGFUNCTION SetSdkLogFunction( LPDSSDK_LOGFUNCTION pFun )
{
    LPDSSDK_LOGFUNCTION pTmpFun = s_pSdkLogFun;
    s_pSdkLogFun = pFun;
    return pTmpFun;
}

VOID SDK_Log(const CHAR* pLogString , ...)
{
    CHAR szLine[1024] = {0};
    va_list	va;
    va_start(va,pLogString);
#ifdef __LINUX
    vsprintf(szLine,pLogString,va);
#else
    vsprintf_s(szLine,sizeof(szLine), pLogString, va);
#endif
    va_end(va);

    if (s_pSdkLogFun)
    {
        s_pSdkLogFun(szLine);
    }
    else
    {
        printf("%s", szLine);
        printf("\n");
    }
}

// 根据角色名生成一个guid，用于debug模式下登录使用
INT CharName2Guid(const CHAR* pCharName, INT nMaxSize)
{
	if(pCharName == NULL)
	{
		return -1;
	}
	INT result = 0;
	INT nIndex = 0;
	while(*(pCharName+nIndex) != '\0' && nIndex < nMaxSize)
	{
		CHAR temp = result >> 24;
		result = result << 8;
		result += temp;
		result += *(pCharName+nIndex);
		nIndex++;
	}

	if (result < 0)
	{
		result *= -1;
	}
	if (result == 0)
	{
		result = rand() % 9999;
	}

	return result;
}

bool IsNaN( FLOAT f )
{
	//int i = 0;
	//static const float fNaN = 1.0 / i;
	////float fNaN = std::numeric_limits<float>::quiet_NaN();
	//return fNaN == f;
    static const float MAX_FLOAT = 3.40282e38f;
    static const float MIN_FLOAT = -3.40282e38f;
    return (f>MAX_FLOAT || f<MIN_FLOAT);
}

bool IsNan( DOUBLE d )
{
	int i = 0;
	static const double dNaN = 1.0 / i;
	//double dNaN = std::numeric_limits<double>::quiet_NaN();
	return dNaN == d;
}

const std::string GetStackString()
{
	std::string stackString;

//#ifdef __LINUX
//	void *array[20];
//	size_t size;
//	char **strings;
//
//	size = backtrace(array, 20);
//	strings = backtrace_symbols(array, size);
//	for(int i=0; i<size; i++)
//	{
//		stackString += strings[i];
//		stackString += "\n";
//	}
//	free(strings);
//#endif

	return stackString;
}
