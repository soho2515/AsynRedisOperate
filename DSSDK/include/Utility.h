/*
文件	：Utility.h
简介	：
作者	：侯志芳
创建日期：2009/12/23

最后修改日期	修改者		修改内容
---------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------
*/

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "BaseType.h"

#if defined(__LINUX)
# include <unistd.h>
#include <pthread.h>
#elif defined(__WINDOWS)
//#include <Windows.h>
#endif

typedef void (*SigHandleCallbackFun)();
typedef VOID (*LPDSSDK_LOGFUNCTION)(const CHAR *);

LPDSSDK_LOGFUNCTION SetSdkLogFunction(LPDSSDK_LOGFUNCTION pFun);

void registSignalHandler(const char* szDumpFileName,  SigHandleCallbackFun pAppExitFun);

#ifdef __WINDOWS
# define MySleep Sleep
#else
# define MySleep(x) usleep((x)*1000)
#endif


//文件与路径相关的工具函数
VOID ParseFilePath(char* szPath);
VOID GetWorkingDir(const int nBuffSize, char* szBuff);
bool CheckDirExsist(const CHAR* szPath);
bool DSCreateDirectory(const CHAR* szPath);
bool CheckFileExsist(const CHAR* szPath);
VOID DSDeleteFile(const CHAR* szPath);

// [start, end)
INT RangeRandom(INT start, INT end);
FLOAT RangeRandom(FLOAT start, FLOAT end);

// 获取一种排列，从n个数里面选出来m个,填入array中，array中的数是为0到n-1的数
VOID GetP( INT m, INT n, INT* array );

//内存转换函数，主要用在存储中
BYTE  HexToByte(CHAR cChar);


/***************************************************************************************************************/
/*  函数名：BinaryToString                                                                                     */
/*  描述  ：将一段内存转换为字符串形式，字符串的表现形式相对于直接看这段内存的样子                             */
/*  参数  ：pBuf 裸内存指针，nBufLen 裸内存的长度，以byte计数，pszBuf存放字符串的指针，nSzBuffLen接收字符串长度*/
/*  返回值：成功返回TRUE,失败返回FALSE                                                                         */
/***************************************************************************************************************/
bool  BinaryToString(const VOID* pBuf, const UINT nBufLen, CHAR* pszBuf, const UINT nSzBuffLen);


/***************************************************************************************************************/
/*  函数名：StringToBinary                                                                                     */
/*  描述  ：将一段由BinaryToString函数转出的字符串回转成内存                                                   */
/*  参数  ：pszBuf 要转换的字符串，pBuf 接收裸内存的指针，nLen 接收裸内存的内存长度，以Byte计数                */
/*  返回值：成功返回TRUE,失败返回FALSE                                                                         */
/***************************************************************************************************************/
bool  StringToBinary(const CHAR* pszBuf, VOID* pBuf, UINT nLen);
bool  StringToBinary(const CHAR* pszBufBegin, const CHAR* pszBufEnd, VOID* pBuf, UINT nLen);

/*
BinaryToString和StringToBinary的使用的例子：
INT nn[10] = {11, 12, 13, 14, 88, 85, -58, 51, 99, 852};
CHAR szBuff[100] = {0};
BinaryToString((PVOID)nn, sizeof(nn), szBuff, 100);
INT output[10] = {0};
StringToBinary(szBuff, (PVOID)output, sizeof(output));

*/

//线程相关的工具函数
unsigned long GetThreadID();

// SDK内部的日志函数
VOID SDK_Log(const CHAR* pLogString , ...);

// 根据角色名生成一个guid，用于debug模式下登录使用
INT CharName2Guid(const CHAR* pCharName, INT nMaxSize);

// 判断浮点数是否有效
bool IsNaN( FLOAT f );
bool IsNan( DOUBLE d );

// 获得函数运行的栈
#include <string>
const std::string GetStackString();

#endif // __UTILITY_H__
