/*************************************************************************
 *                                                                       *
 *  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
 *  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
 *        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
 *       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
 *     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
 *    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
 *   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
 * !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
 *                                                                       *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/

/**
 * @file
 * @brief MD5计算相关
 * 
 *      计算文件/字符串MD5
 *      本文件中包含2个类
 *      ZLMd5BaseImpl   原始的MD5算法,摘至Qt源码的qt\src\3rdparty\md5,有删改
 *      ZLMd5           封装md5算法,实现对文件和字符串计算MD5的简单接口
 */

#pragma once
#include <string>
#include "file_info.hpp" // GetFileSize()

#define ZLMD5_MAX_READ_BYTES 1048576 // 1024 * 1024 -> 1MB
#define ZLMD5_F1(x, y, z) (z ^ (x & (y ^ z)))
#define ZLMD5_F2(x, y, z) ZLMD5_F1(z, x, y)
#define ZLMD5_F3(x, y, z) (x ^ y ^ z)
#define ZLMD5_F4(x, y, z) (y ^ (x | ~z))
#define ZLMD5_MD5STEP(f,w,x,y,z,in,s) (w += f(x,y,z) + in, w = (w<<s | w>>(32-s)) + x)


namespace zl
{
namespace WinUtils
{

/**
 * @brief 获取MD5
 * @note 计算文件MD5时,参数nMaxLimitMb为限制的最大文件大小(MB),当大于这个大小时,将不计算MD5结果
 */
class ZLMd5
{
public:
    static CString GetFileMd5(LPCTSTR lpFilePath, LONGLONG ulMaxFileBytes = 1024*1024*4);
    static BOOL    GetFileMd5(LPCTSTR lpFilePath, unsigned char arr[16], LONGLONG ulMaxFileBytes = 1024*1024*4);
    static CString GetBufMd5(const unsigned char* pBuf, unsigned int uLen);
    static BOOL    GetBufMd5(const unsigned char* pBuf, unsigned int uLen, unsigned char arr[16]);
};


/**
 * @brief MD5算法实现,摘至Qt源码的qt\src\3rdparty\md5,有删改
 */
class ZLMd5BaseImpl
{
public:
    void MD5Init();
    void MD5Update(const unsigned char* pBuf, unsigned int uLen);
    void MD5Final(unsigned char digest[16]);

private:
    void MD5Transform(unsigned int buf[4], unsigned int const in[16]) const;
    void _ByteSwap(unsigned int *buf, unsigned int words);

private:
    struct MD5Context {
        unsigned int buf[4];
        unsigned int bytes[2];
        unsigned int in[16];
    } m_stMd5Context;
};


// 以下是实现部分

inline CString ZLMd5::GetFileMd5( LPCTSTR lpFilePath, LONGLONG ulMaxFileBytes )
{
    CString sMd5;
    unsigned char arrA[16] = {0};
    if (GetFileMd5(lpFilePath, arrA, ulMaxFileBytes))
    {
        for (int i = 0; i < 16; ++i)
        {
            sMd5.AppendFormat(_T("%02x"), arrA[i]);
        }
    }
    return sMd5;
}

inline BOOL ZLMd5::GetFileMd5( LPCTSTR lpFilePath, unsigned char arr[16], LONGLONG ulMaxFileBytes )
{
    LONGLONG ulSize = 0;
    BOOL bRet = zl::WinUtils::ZLFileInfo::GetFileSize(lpFilePath, ulSize);
    if (FALSE == bRet || ulSize >= ulMaxFileBytes)
    {
        return FALSE;
    }

    FILE *fp = NULL;
    _tfopen_s(&fp, lpFilePath, _T("rb"));
    if (NULL == fp)
    {
        return FALSE;
    }

    ZLMd5BaseImpl impl;
    impl.MD5Init();
    unsigned char* pBuf = new unsigned char[ZLMD5_MAX_READ_BYTES];
    size_t nRead = 0;
    while (nRead = fread(pBuf,  sizeof(unsigned char), ZLMD5_MAX_READ_BYTES, fp))
    {
        impl.MD5Update(pBuf, (unsigned)nRead);
    }

    delete pBuf;
    ::fclose(fp);
    impl.MD5Final(arr);

    return TRUE;
}

inline CString ZLMd5::GetBufMd5( const unsigned char* pBuf, unsigned int uLen )
{
    CString sMd5;
    unsigned char arrA[16] = {0};
    if (GetBufMd5(pBuf, uLen, arrA))
    {
        for (int i = 0; i < 16; ++i)
        {
            sMd5.AppendFormat(_T("%02x"), arrA[i]);
        }
    }
    return sMd5;
}

inline BOOL ZLMd5::GetBufMd5( const unsigned char* pBuf, unsigned int uLen, unsigned char arr[16] )
{
    if (pBuf && uLen > 0)
    {
        ZLMd5BaseImpl impl;
        impl.MD5Init();
        impl.MD5Update(pBuf, (unsigned)uLen);
        impl.MD5Final(arr);
        return TRUE;
    }
    return FALSE;
}

inline void ZLMd5BaseImpl::MD5Update( const unsigned char* pBuf, unsigned int uLen )
{
    unsigned int t;

    /* Update byte count */
    t = m_stMd5Context.bytes[0];
    if ((m_stMd5Context.bytes[0] = t + uLen) < t)
        m_stMd5Context.bytes[1]++;	/* Carry from low to high */

    t = 64 - (t & 0x3f);	/* Space available in ctx->in (at least 1) */
    if (t > uLen) {
        memcpy((unsigned char *)m_stMd5Context.in + 64 - t, pBuf, uLen);
        return;
    }
    /* First chunk is an odd size */
    memcpy((unsigned char *)m_stMd5Context.in + 64 - t, pBuf, t);
    _ByteSwap(m_stMd5Context.in, 16);
    MD5Transform(m_stMd5Context.buf, m_stMd5Context.in);
    pBuf += t;
    uLen -= t;

    /* Process data in 64-byte chunks */
    while (uLen >= 64) {
        memcpy(m_stMd5Context.in, pBuf, 64);
        _ByteSwap(m_stMd5Context.in, 16);
        MD5Transform(m_stMd5Context.buf, m_stMd5Context.in);
        pBuf += 64;
        uLen -= 64;
    }

    /* Handle any remaining bytes of data. */
    memcpy(m_stMd5Context.in, pBuf, uLen);
}

inline void ZLMd5BaseImpl::MD5Init()
{
	m_stMd5Context.buf[0] = 0x67452301;
	m_stMd5Context.buf[1] = 0xefcdab89;
	m_stMd5Context.buf[2] = 0x98badcfe;
	m_stMd5Context.buf[3] = 0x10325476;

	m_stMd5Context.bytes[0] = 0;
	m_stMd5Context.bytes[1] = 0;
}

inline void ZLMd5BaseImpl::MD5Final(unsigned char digest[16])
{
	int count = m_stMd5Context.bytes[0] & 0x3f;	/* Number of bytes in ctx->in */
	unsigned char *p = (unsigned char *)m_stMd5Context.in + count;

	/* Set the first char of padding to 0x80.  There is always room. */
	*p++ = 0x80;

	/* Bytes of padding needed to make 56 bytes (-8..55) */
	count = 56 - 1 - count;

	if (count < 0) {	/* Padding forces an extra block */
		memset(p, 0, count + 8);
		_ByteSwap(m_stMd5Context.in, 16);
		MD5Transform(m_stMd5Context.buf, m_stMd5Context.in);
		p = (unsigned char *)m_stMd5Context.in;
		count = 56;
	}
	memset(p, 0, count);
	_ByteSwap(m_stMd5Context.in, 14);

	/* Append length in bits and transform */
	m_stMd5Context.in[14] = m_stMd5Context.bytes[0] << 3;
	m_stMd5Context.in[15] = m_stMd5Context.bytes[1] << 3 | m_stMd5Context.bytes[0] >> 29;
	MD5Transform(m_stMd5Context.buf, m_stMd5Context.in);

	_ByteSwap(m_stMd5Context.buf, 4);
	memcpy(digest, m_stMd5Context.buf, 16);
	memset(&m_stMd5Context, 0, sizeof(m_stMd5Context));	/* In case it's sensitive */
}

inline void ZLMd5BaseImpl::MD5Transform(unsigned int buf[4], unsigned int const in[16]) const
{
	register unsigned int a, b, c, d;

	a = buf[0];
	b = buf[1];
	c = buf[2];
	d = buf[3];

	ZLMD5_MD5STEP(ZLMD5_F1, a, b, c, d, in[0] + 0xd76aa478, 7);
	ZLMD5_MD5STEP(ZLMD5_F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
	ZLMD5_MD5STEP(ZLMD5_F1, c, d, a, b, in[2] + 0x242070db, 17);
	ZLMD5_MD5STEP(ZLMD5_F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
	ZLMD5_MD5STEP(ZLMD5_F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
	ZLMD5_MD5STEP(ZLMD5_F1, d, a, b, c, in[5] + 0x4787c62a, 12);
	ZLMD5_MD5STEP(ZLMD5_F1, c, d, a, b, in[6] + 0xa8304613, 17);
	ZLMD5_MD5STEP(ZLMD5_F1, b, c, d, a, in[7] + 0xfd469501, 22);
	ZLMD5_MD5STEP(ZLMD5_F1, a, b, c, d, in[8] + 0x698098d8, 7);
	ZLMD5_MD5STEP(ZLMD5_F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
	ZLMD5_MD5STEP(ZLMD5_F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
	ZLMD5_MD5STEP(ZLMD5_F1, b, c, d, a, in[11] + 0x895cd7be, 22);
	ZLMD5_MD5STEP(ZLMD5_F1, a, b, c, d, in[12] + 0x6b901122, 7);
	ZLMD5_MD5STEP(ZLMD5_F1, d, a, b, c, in[13] + 0xfd987193, 12);
	ZLMD5_MD5STEP(ZLMD5_F1, c, d, a, b, in[14] + 0xa679438e, 17);
	ZLMD5_MD5STEP(ZLMD5_F1, b, c, d, a, in[15] + 0x49b40821, 22);

	ZLMD5_MD5STEP(ZLMD5_F2, a, b, c, d, in[1] + 0xf61e2562, 5);
	ZLMD5_MD5STEP(ZLMD5_F2, d, a, b, c, in[6] + 0xc040b340, 9);
	ZLMD5_MD5STEP(ZLMD5_F2, c, d, a, b, in[11] + 0x265e5a51, 14);
	ZLMD5_MD5STEP(ZLMD5_F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
	ZLMD5_MD5STEP(ZLMD5_F2, a, b, c, d, in[5] + 0xd62f105d, 5);
	ZLMD5_MD5STEP(ZLMD5_F2, d, a, b, c, in[10] + 0x02441453, 9);
	ZLMD5_MD5STEP(ZLMD5_F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
	ZLMD5_MD5STEP(ZLMD5_F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
	ZLMD5_MD5STEP(ZLMD5_F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
	ZLMD5_MD5STEP(ZLMD5_F2, d, a, b, c, in[14] + 0xc33707d6, 9);
	ZLMD5_MD5STEP(ZLMD5_F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
	ZLMD5_MD5STEP(ZLMD5_F2, b, c, d, a, in[8] + 0x455a14ed, 20);
	ZLMD5_MD5STEP(ZLMD5_F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
	ZLMD5_MD5STEP(ZLMD5_F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
	ZLMD5_MD5STEP(ZLMD5_F2, c, d, a, b, in[7] + 0x676f02d9, 14);
	ZLMD5_MD5STEP(ZLMD5_F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

	ZLMD5_MD5STEP(ZLMD5_F3, a, b, c, d, in[5] + 0xfffa3942, 4);
	ZLMD5_MD5STEP(ZLMD5_F3, d, a, b, c, in[8] + 0x8771f681, 11);
	ZLMD5_MD5STEP(ZLMD5_F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
	ZLMD5_MD5STEP(ZLMD5_F3, b, c, d, a, in[14] + 0xfde5380c, 23);
	ZLMD5_MD5STEP(ZLMD5_F3, a, b, c, d, in[1] + 0xa4beea44, 4);
	ZLMD5_MD5STEP(ZLMD5_F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
	ZLMD5_MD5STEP(ZLMD5_F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
	ZLMD5_MD5STEP(ZLMD5_F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
	ZLMD5_MD5STEP(ZLMD5_F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
	ZLMD5_MD5STEP(ZLMD5_F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
	ZLMD5_MD5STEP(ZLMD5_F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
	ZLMD5_MD5STEP(ZLMD5_F3, b, c, d, a, in[6] + 0x04881d05, 23);
	ZLMD5_MD5STEP(ZLMD5_F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
	ZLMD5_MD5STEP(ZLMD5_F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
	ZLMD5_MD5STEP(ZLMD5_F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
	ZLMD5_MD5STEP(ZLMD5_F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

	ZLMD5_MD5STEP(ZLMD5_F4, a, b, c, d, in[0] + 0xf4292244, 6);
	ZLMD5_MD5STEP(ZLMD5_F4, d, a, b, c, in[7] + 0x432aff97, 10);
	ZLMD5_MD5STEP(ZLMD5_F4, c, d, a, b, in[14] + 0xab9423a7, 15);
	ZLMD5_MD5STEP(ZLMD5_F4, b, c, d, a, in[5] + 0xfc93a039, 21);
	ZLMD5_MD5STEP(ZLMD5_F4, a, b, c, d, in[12] + 0x655b59c3, 6);
	ZLMD5_MD5STEP(ZLMD5_F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
	ZLMD5_MD5STEP(ZLMD5_F4, c, d, a, b, in[10] + 0xffeff47d, 15);
	ZLMD5_MD5STEP(ZLMD5_F4, b, c, d, a, in[1] + 0x85845dd1, 21);
	ZLMD5_MD5STEP(ZLMD5_F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
	ZLMD5_MD5STEP(ZLMD5_F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
	ZLMD5_MD5STEP(ZLMD5_F4, c, d, a, b, in[6] + 0xa3014314, 15);
	ZLMD5_MD5STEP(ZLMD5_F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
	ZLMD5_MD5STEP(ZLMD5_F4, a, b, c, d, in[4] + 0xf7537e82, 6);
	ZLMD5_MD5STEP(ZLMD5_F4, d, a, b, c, in[11] + 0xbd3af235, 10);
	ZLMD5_MD5STEP(ZLMD5_F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
	ZLMD5_MD5STEP(ZLMD5_F4, b, c, d, a, in[9] + 0xeb86d391, 21);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

inline void ZLMd5BaseImpl::_ByteSwap( unsigned int *buf, unsigned int words )
{
    const unsigned int byteOrderTest = 0x1;
    if (((char *)&byteOrderTest)[0] == 0) {
        unsigned char *p = (unsigned char *)buf;

        do {
            *buf++ = (unsigned int)((unsigned)p[3] << 8 | p[2]) << 16 |
                ((unsigned)p[1] << 8 | p[0]);
            p += 4;
        } while (--words);
    }
}

} // end of namespace WinUtils
} // end of namespace zl
