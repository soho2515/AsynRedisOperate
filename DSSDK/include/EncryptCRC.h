//----------------------------------------------------------------------------------------------------------------
//	数据的实时压缩和CRC32校验功能
//
//----------------------------------------------------------------------------------------------------------------

#ifndef __ENCRYPTCRC_H__
#define __ENCRYPTCRC_H__

#ifdef __WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // 防止windows.h里包含winsock.h

# include <windows.h>
#else
# include "BaseType.h"
# include <string.h>
#define strcpy_s strcpy
#endif

#ifdef _MSC_VER

#include <stdlib.h>
#pragma intrinsic(_lrotr,_lrotl)
#define rotr(x,n) _lrotr(x,n)
#define rotl(x,n) _lrotl(x,n)

#else

#define rotr(x,n)   (((x) >> ((int)(n))) | ((x) << (32 - (int)(n))))
#define rotl(x,n)   (((x) << ((int)(n))) | ((x) >> (32 - (int)(n))))

#endif

class CEncryptCRC  
{
public:
	CEncryptCRC();
	virtual ~CEncryptCRC();
private:
    unsigned int	m_dwkey[44];
	int     m_cninit;
private:
	void	Encrypt(const BYTE in_blk[16], BYTE out_blk[16]);
	void	Decrypt(const BYTE in_blk[16], BYTE out_blk[16]);
public:

	//得到加密算法名
	char *name(void);

	//计算指定pData的CRC32值
	//dwInitCrc32为先前的CRC32值 可以把先前CalcCrc32结果传送进来
	//继续计算下一个内存缓冲数据的CRC32值，这样就可以一个大的数据块分为多次CalcCrc32
	//而最后得到一个CRC32值
	unsigned int CalcCrc32(LPVOID pData,int cnDataLen,unsigned int dwInitCrc32=0);

	//设置加密的密钥  pszKey为字符串密钥  dwKeySize为密钥长度,(多少位  一个字符8位)
	void SetCryptKey(const char	*pszKey,const unsigned int dwKeySize);

	//加密指定数据,函数返回加密后的数据长度
	//Data本身必须有足够的空间容纳加密后数据,
	//注意加密后的长度不变
	//要达到良好的加密效果DataLen尽量>16个字符
	int EncryptData(void *Data,int DataLen);
    
	//解密指定数据,函数返回解密后的数据长度
	//Data本身必须有足够的空间容纳解密后数据,注意解密后的长度不变
	int DecryptData(void *Data,int DataLen);
};

#endif // __ENCRYPTCRC_H__

