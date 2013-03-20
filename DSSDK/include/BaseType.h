#ifndef __BASETYPE_H__
#define __BASETYPE_H__

#pragma warning (disable: 4996)
#pragma warning (disable: 4244)   //这个以后可以适当放开，以检查类型转换的loss of data的问题。

#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif
#define FD_SETSIZE (2048)


typedef char				CHAR;
typedef unsigned char		UCHAR;
typedef unsigned char       BYTE;

typedef short				SHORT;
typedef unsigned short		USHORT;

typedef int                 INT;
typedef unsigned int        UINT;

typedef long				LONG;
typedef long long			LONGLONG;
typedef unsigned long		ULONG;
typedef unsigned long       u_long;

#ifndef __LINUX
typedef unsigned int        size_t;
#endif

//typedef int                 bool;

typedef float               FLOAT;
typedef double              DOUBLE;

typedef void               *LPVOID;

#ifndef VOID
# define VOID void
#endif

#ifndef NULL
#define NULL    0
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define     DS_INVALID_ID  (-1)

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p) = NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { p->release(); (p) = NULL; }}
#endif

#ifdef __WINDOWS
#define  ERRORNO WSAGetLastError()
#else
#define ERRORNO	errno
#endif

// socket ，句柄 定义

#if defined (__WINDOWS)
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif // 防止windows.h里包含winsock.h

#   include <wtypes.h>
#   include <WinSock.h>
#endif

#if defined (__WINDOWS)

typedef HANDLE DS_HANDLE;
typedef SOCKET DS_SOCKET;

# define DS_INVALID_HANDLE INVALID_HANDLE_VALUE

#else

typedef int DS_HANDLE;
typedef DS_HANDLE DS_SOCKET;

#ifdef INVALID_HANDLE_VALUE
#undef INVALID_HANDLE_VALUE
#endif
#define INVALID_HANDLE_VALUE (-1)


# define DS_INVALID_HANDLE (-1)
# define SOCKET_ERROR (-1)

#ifdef INVALID_SOCKET
#undef INVALID_SOCKET
#endif
#define INVALID_SOCKET (DS_SOCKET)(~0)


#endif /* __WINDOWS */


#ifdef __LINUX
# ifndef _snprintf
#  define _snprintf snprintf
# endif
# ifndef strnicmp
#  define strnicmp strncasecmp
# endif
#endif

#ifndef MAX_PATH
#define MAX_PATH  256
#endif

#endif // __BASETYPE_H__
