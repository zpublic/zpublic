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
* @brief 字符串转换
*/


#pragma once
#include <atlstr.h>

namespace zl
{
namespace WinUtils
{

/**
 * @brief 字符串转换
 */
class ZLStrConv
{
public:
    /**
     * @brief 简单字符串转换
     * @param[in] lpBuf      需要转制的字符串
     * @param[in] uCodePage  Code Page
     * @param[in] nBufLen    字符串长度,如果字符串以0结尾,则此参数可填-1
     * @return 返回转换后的字符串,为空字符串时转换失败.
     */
    static CStringW a2w(LPCSTR  lpBuf, UINT uCodePage = CP_ACP, int nBufLen = -1);
    static CStringA w2a(LPCWSTR lpBuf, UINT uCodePage = CP_ACP, int nBufLen = -1);
    
    /**
     * @brief 长度很大的字符串转换,如文件内容
     * @param[in] lpSrc      需要转制的字符串
     * @param[in] nSrcLen    字符串长度
     * @param[in] lpDstW     用于存储转换后的字符串的缓存区指针,为NULL时,函数返回实际需要的缓存区长度
     * @param[in] nDstLen    缓存区长度
     * @param[in] uCodePage  Code Page
     * @return 成功返回实际转换长度,小于等于0时失败.
     * @see MultiByteToWideChar,WideCharToMultiByte
     */
    static int a2w(LPCSTR  lpSrcA, int nSrcLen, LPWSTR lpDstW, int nDstLen, UINT uCodePage = CP_ACP);
    static int w2a(LPCWSTR lpSrcW, int nSrcLen, LPSTR  lpDstA, int nDstLen, UINT uCodePage = CP_ACP);
};

#define ZLA2W zl::WinUtils::ZLStrConv::a2w
#define ZLW2A zl::WinUtils::ZLStrConv::w2a


// 下面是实现部分

inline CStringW ZLStrConv::a2w(LPCSTR lpBuf, UINT uCodePage, int nBufLen)
{
    CStringW sRetW;
    int nDstLen = ::MultiByteToWideChar( uCodePage, 0, lpBuf, nBufLen, NULL, 0 );
    if (nDstLen > 0)
    {
        wchar_t* pResult = new wchar_t[nDstLen];
        BOOL bSuc = (0 < ::MultiByteToWideChar(uCodePage, 0, lpBuf, nBufLen, pResult, nDstLen));
        if (bSuc)
        {
            sRetW.Append(pResult, nDstLen - 1);
        }
        delete[] pResult;
    }
    return sRetW;
}

inline CStringA ZLStrConv::w2a( LPCWSTR lpBuf, UINT uCodePage, int nBufLen )
{
    CStringA sRetA;
    int nDstLen = ::WideCharToMultiByte( uCodePage, 0, lpBuf, nBufLen, NULL, 0, NULL, NULL );
    if (nDstLen > 0)
    {
        char* pResult = new char[nDstLen];
        BOOL bSuc = (0 < ::WideCharToMultiByte( uCodePage, 0, lpBuf, nBufLen, pResult, nDstLen, NULL, NULL ));
        if (bSuc)
        {
            sRetA.Append(pResult, nDstLen - 1);
        }
        delete[] pResult;
    }
    return sRetA;
}

inline int ZLStrConv::a2w( LPCSTR lpSrcA, int nSrcLen, LPWSTR lpDstW, int nDstLen, UINT uCodePage )
{
    return ::MultiByteToWideChar(uCodePage, 0, lpSrcA, nSrcLen, lpDstW, nDstLen);
}

inline int ZLStrConv::w2a( LPCWSTR lpSrcW, int nSrcLen, LPSTR lpDstA, int nDstLen, UINT uCodePage )
{
    return ::WideCharToMultiByte(uCodePage, 0, lpSrcW, nSrcLen, lpDstA, nDstLen, NULL, NULL);
}

}
}