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
 * @brief 错误码相关
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{

    class ZLErrorCode
    {
    public:
        ///> need LocalFree(pBuffer);
        /**
         * @brief 获得ErrorCode对应的描述信息
         * @param[in] lpLnkFilePath 错误码
         * @return 成功返回错误码对应的描述信息，失败返回NULL
         * @see _ExpandFullPathName
         */
        static LPTSTR GetFormattedMessage(DWORD dwErrCode = ::GetLastError())
        {
            LPTSTR lpMsgBuf = NULL;
            ::FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dwErrCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                (LPTSTR) &lpMsgBuf,
                0,
                NULL);
            return lpMsgBuf;
        }
    };

}
}
