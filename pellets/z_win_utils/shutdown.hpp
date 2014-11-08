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
 * @brief 重启和关机
 */


#pragma once
#include "win_utils_header.h"
#include "privilege.hpp"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 重启和关机
     */
    class ZLShutdown
    {
    public:
        static BOOL RebootSystem()
        {
            BOOL bRet = FALSE;
            ZLPrivilege::GetPrivileges(SE_SHUTDOWN_NAME);
            bRet = ::ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_FLAG_PLANNED);
            return bRet;
        }

        static BOOL LogOffSystem()
        {
            BOOL bRet = FALSE;
            ZLPrivilege::GetPrivileges(SE_SHUTDOWN_NAME);
            bRet = ::ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, SHTDN_REASON_FLAG_PLANNED);
            return bRet;
        }

        static BOOL PowerOffSystem()
        {
            BOOL bRet = FALSE;
            ZLPrivilege::GetPrivileges(SE_SHUTDOWN_NAME);
            bRet = ::ExitWindowsEx(EWX_SHUTDOWN | EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_FLAG_PLANNED);
            return bRet;
        }
    };

}
}
