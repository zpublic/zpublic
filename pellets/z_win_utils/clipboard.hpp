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
 * @brief 剪切板操作相关
 */


#pragma once
#include "win_utils_header.h"
#include <string.h>

namespace zl
{
namespace WinUtils
{
	/**
	* @brief 剪切板操作类，可以设置剪切板数据，获取剪切板数据
	*/
    class ZLClipboard
    {
    public:
		/**
		 * @brief 向剪切板设置数据
		 * @param[in]   pszData Buffer，包含要设置到剪切板的数据
		 * @param[in]   nDataLen 设置数据的长度
		 * @return 如果成功，返回TRUE，如果失败返回FALSE
		 */
        static BOOL SetClipboard(const char* pszData, const int nDataLen)
        {
            if (::OpenClipboard(NULL))
            {
                ::EmptyClipboard();
                HGLOBAL hMem = ::GlobalAlloc(GMEM_DDESHARE, nDataLen + 1);
                if (hMem)
                {
                    char *buffer = (char *)::GlobalLock(hMem);
                    strcpy_s(buffer, nDataLen + 1, pszData);
                    ::GlobalUnlock(hMem);
                    ::SetClipboardData(CF_TEXT, hMem);
                }
                ::CloseClipboard();
                return TRUE;
            }
            return FALSE;
        }
		/**
		 * @brief 获取剪切板的数据
		 * @return 如果成功，返回剪切板中的数据，如果失败返回空串
		 */
        static CStringA GetClipboard()
        {
            CStringA sText;
            if (::IsClipboardFormatAvailable(CF_TEXT) && ::OpenClipboard(NULL))
            {
                HGLOBAL hMem = ::GetClipboardData(CF_TEXT);
                if (hMem)
                {
                    LPSTR lpStr = (LPSTR)::GlobalLock(hMem);
                    if (lpStr)
                    {
                        sText = lpStr;
                        ::GlobalUnlock(hMem);
                    }
                }
                ::CloseClipboard();
            }
            return sText;
        }
    };

}
}
