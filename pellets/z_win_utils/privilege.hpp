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
 * @brief 进程权限相关
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 调整进程权限
     */
    class ZLPrivilege
    {
    public:
        /**
        * @brief 调整当前进程权限
        * @param[in] szPrivileges 权限名
        * @return 成功返回TRUE，失败返回FALSE
        * @see OpenProcessToken LookupPrivilegeValue AdjustTokenPrivileges
        */
        static BOOL GetPrivileges(LPCTSTR szPrivileges = SE_DEBUG_NAME)
        {
            BOOL bRet = FALSE;
            HANDLE hToken = NULL;
            LUID CurrentLUID = { 0 };
            TOKEN_PRIVILEGES TokenPrivileges = { 0 };
            __try
            {
                bRet = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
                if (!bRet) __leave;

                bRet = ::LookupPrivilegeValue(NULL, szPrivileges, &CurrentLUID);
                if (!bRet) __leave;

                TokenPrivileges.PrivilegeCount = 1;
                TokenPrivileges.Privileges[0].Luid = CurrentLUID;
                TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                bRet = ::AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, 0, NULL, NULL);
                if (!bRet) __leave;

                bRet = TRUE;
            }
            __finally
            {
                if (hToken)
                {
                    ::CloseHandle(hToken);
                    hToken = NULL;
                }
            }
            return bRet;
        }
    };

}
}
