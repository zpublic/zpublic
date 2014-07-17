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
 * @brief 系统启动项
 */


#pragma once
#include "win_utils_header.h"
#include "usid.hpp"
#include "register.hpp"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 提供对windows常见自启动项的增,删,改操作
     */
    class ZLAutorun
    {
    public:
        enum UserType { ALL_USER, SINGLE_USER };

        /**
         * @brief 注册表run中添加一个启动项
         * @param[in] t         指定处理AllUser还是单个用户,若此处填SINGLE_USER,则通过参数lpUserSid指定用户
         * @param[in] lpValName 值名
         * @param[in] lpVal     值内容
         * @param[in] dwValType 值类型
         * @param[in] lpUserSid 用户SID, 填NULL时, 为当前用户SID.此值仅在参数t为SINGLE_USER时有效
         * @param[in] bWow64    64位重定向
         * @return 成功返回TRUE, 失败返回FALSE
         */
        static BOOL AddRegRun(
            UserType    t,
            LPCTSTR     lpValName,
            LPCTSTR     lpVal,
            DWORD       dwValType = REG_EXPAND_SZ,
            LPCTSTR     lpUserSid = NULL,
            BOOL        bWow64    = TRUE)
        {
            if (!lpValName || !lpVal)
                return FALSE;

            BOOL bReturn = FALSE;
            LPCTSTR lpKeyBase = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
            switch (t)
            {
            case ALL_USER:
                bReturn = _SetRegStringAllUser(lpKeyBase, lpValName, lpVal, dwValType, bWow64);
                break;
            case SINGLE_USER:
                bReturn = _SetRegStringSingleUser(lpUserSid, lpKeyBase, lpValName, lpVal, dwValType, bWow64);
                break;
            }
            return bReturn;
        }

        /**
         * @brief 注册表runonce中添加一个启动项
         * @see AddRegRun
         */
        static BOOL AddRegRunOnce(
            UserType    t,
            LPCTSTR     lpValName,
            LPCTSTR     lpVal,
            DWORD       dwValType = REG_EXPAND_SZ,
            LPCTSTR     lpUserSid = NULL,
            BOOL        bWow64    = TRUE)
        {
            if (!lpValName || !lpVal)
                return FALSE;

            BOOL bReturn = FALSE;
            LPCTSTR lpKeyBase = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Runonce");
            switch (t)
            {
            case ALL_USER:
                bReturn = _SetRegStringAllUser(lpKeyBase, lpValName, lpVal, dwValType, bWow64);
                break;
            case SINGLE_USER:
                bReturn = _SetRegStringSingleUser(lpUserSid, lpKeyBase, lpValName, lpVal, dwValType, bWow64);
                break;
            }
            return bReturn;
        }

        /**
         * @brief 删除注册表run下指定的启动项
         * @param[in] t         指定处理AllUser还是单个用户,若此处填SINGLE_USER,则通过参数lpUserSid指定用户
         * @param[in] lpValName 值名
         * @param[in] lpUserSid 用户SID, 填NULL时, 为当前用户SID.此值仅在参数t为SINGLE_USER时有效
         * @param[in] bWow64    64位重定向
         * @return 成功返回TRUE, 失败返回FALSE
         */
        static BOOL DelRegRun(
            UserType    t,
            LPCTSTR     lpValName,
            LPCTSTR     lpUserSid = NULL,
            BOOL        bWow64    = TRUE)
        {
            BOOL    bReturn   = FALSE;
            LPCTSTR lpKeyBase = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
            switch (t)
            {
            case ALL_USER:
                bReturn = _DelRegStringAllUser(lpKeyBase, lpValName, bWow64);
                break;
            case SINGLE_USER:
                bReturn = _DelRegStringSingleUser(lpUserSid, lpKeyBase, lpValName, bWow64);
                break;
            default:
                break;
            }
            return bReturn;
        }

        /**
         * @brief 删除注册表runonce下指定的启动项
         * @see DelRegRun
         */
        static BOOL DelRegRunOnce(
            UserType    t,
            LPCTSTR     lpValName,
            LPCTSTR     lpUserSid = NULL,
            BOOL        bWow64    = TRUE)
        {
            BOOL    bReturn   = FALSE;
            LPCTSTR lpKeyBase = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Runonce");
            switch (t)
            {
            case ALL_USER:
                bReturn = _DelRegStringAllUser(lpKeyBase, lpValName, bWow64);
                break;
            case SINGLE_USER:
                bReturn = _DelRegStringSingleUser(lpUserSid, lpKeyBase, lpValName, bWow64);
                break;
            default:
                break;
            }
            return bReturn;
        }

    private:
        static BOOL _SetRegStringAllUser(
            LPCTSTR lpSubKey,
            LPCTSTR lpValName,
            LPCTSTR lpVal,
            DWORD   dwValType,
            BOOL    bWow64)
        {
            BOOL bReturn = FALSE;
            
            ZLRegister reg;
            if (reg.Open(HKEY_LOCAL_MACHINE, lpSubKey, TRUE, bWow64 ? KEY_WRITE : KEY_WOW64_64KEY | KEY_WRITE))
            {
                if (REG_SZ == dwValType)
                {
                    bReturn = reg.Write(lpValName, lpVal);
                }
                else if (REG_EXPAND_SZ == dwValType)
                {
                    bReturn = reg.WriteExpandString(lpValName, lpVal);
                }
                reg.Close();
            }
            return bReturn;
        }

        static BOOL _SetRegStringSingleUser(
            LPCTSTR lpUserSid,
            LPCTSTR lpSubKey,
            LPCTSTR lpValName,
            LPCTSTR lpVal,
            DWORD   dwValType,
            BOOL    bWow64)
        {
            BOOL    bReturn = FALSE;
            CString sUserSid;
            if (lpUserSid)
            {
                sUserSid = lpUserSid;
            }
            else if (ZLUsid::GetCurrentUserSID(sUserSid))
            {
            }
            else
            {
                goto Exit0;
            }

            {
                CString sSubKey;
                sSubKey.Format(_T("%s\\%s"), sUserSid, lpSubKey);
                ZLRegister reg;
                if (reg.Open(HKEY_USERS, sSubKey, TRUE, bWow64 ? KEY_WRITE : KEY_WOW64_64KEY | KEY_WRITE))
                {
                    if (REG_SZ == dwValType)
                    {
                        bReturn = reg.Write(lpValName, lpVal);
                    }
                    else if (REG_EXPAND_SZ == dwValType)
                    {
                        bReturn = reg.WriteExpandString(lpValName, lpVal);
                    }
                    reg.Close();
                }
            }
Exit0:
            return bReturn;
        }

        static BOOL _DelRegStringAllUser(LPCTSTR lpSubKey ,LPCTSTR lpValName, BOOL bWow64)
        {
            BOOL bReturn = FALSE;
            ZLRegister reg;
            if (reg.Open(HKEY_LOCAL_MACHINE, lpSubKey, FALSE, bWow64 ? KEY_WRITE : KEY_WOW64_64KEY | KEY_WRITE))
            {
                bReturn = reg.DeleteValue(lpValName);
                reg.Close();
            }
            return bReturn;
        }

        static BOOL _DelRegStringSingleUser(LPCTSTR lpUserSid, LPCTSTR lpSubKey, LPCTSTR lpValName, BOOL bWow64)
        {
            BOOL bReturn = FALSE;
            CString sUserSid;
            if (lpUserSid)
            {
                sUserSid = lpUserSid;
            }
            else if (ZLUsid::GetCurrentUserSID(sUserSid))
            {
            }
            else
            {
                goto Exit0;
            }

            {
                ZLRegister reg;
                CString sSubKey;
                sSubKey.Format(_T("%s\\%s"), sUserSid, lpSubKey);
                if (reg.Open(HKEY_USERS, sSubKey, FALSE, bWow64 ? KEY_WRITE : KEY_WOW64_64KEY | KEY_WRITE))
                {
                    bReturn = reg.DeleteValue(lpValName);
                    reg.Close();
                }
            }

Exit0:
            return bReturn;
        }
    };

}
}
