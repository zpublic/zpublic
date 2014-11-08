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
 * @brief 环境变量相关
 */


#pragma once
#include "win_utils_header.h"
#include "register.hpp"
#include "usid.hpp"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 环境变量常用操作
     */
    class ZLEnvironmentVar
    {
    public:
        enum EnvironmentType { SYSTEM_ENV, USER_ENV };

        /**
         * @brief 添加一个环境变量
         * @param[in] t         指定是环境变量类型, 系统环境变量(SYSTEM_ENV)或用户环境变量(USER_ENV)
         * @param[in] lpVarName 环境变量名
         * @param[in] lpVar     环境变量值
         * @return 成功返回TRUE, 失败返回FALSE
         */
        static BOOL Add(EnvironmentType t, LPCTSTR lpVarName, LPCTSTR lpVar);

        /**
         * @brief 删除指定环境变量
         * @param[in] t         指定是环境变量类型, 系统环境变量(SYSTEM_ENV)或用户环境变量(USER_ENV)
         * @param[in] lpVarName 环境变量名
         * @return 成功返回TRUE, 失败返回FALSE
         */
        static BOOL Del(EnvironmentType t, LPCTSTR lpVarName);

    private:
        static BOOL _AddSysEnv(LPCTSTR lpVarName, LPCTSTR lpVar);
        static BOOL _DelSysEnv(LPCTSTR lpVarName);
        static BOOL _AddUserEnv(LPCTSTR lpVarName, LPCTSTR lpVar);
        static BOOL _DelUserEnv(LPCTSTR lpVarName);
    };

    inline BOOL ZLEnvironmentVar::Add( EnvironmentType t, LPCTSTR lpVarName, LPCTSTR lpVar )
    {
        if (!lpVarName || !lpVar)
            return FALSE;

        switch (t)
        {
        case SYSTEM_ENV:
            return _AddSysEnv(lpVarName, lpVar);
        case USER_ENV:
            return _AddUserEnv(lpVarName, lpVar);
        }
        return FALSE;
    }

    inline BOOL ZLEnvironmentVar::Del( EnvironmentType t, LPCTSTR lpVarName )
    {
        if (NULL == lpVarName)
            return FALSE;

        switch (t)
        {
        case SYSTEM_ENV:
            return _DelSysEnv(lpVarName);
        case USER_ENV:
            return _DelUserEnv(lpVarName);
        }
        return FALSE;
    }

    inline BOOL ZLEnvironmentVar::_AddSysEnv( LPCTSTR lpVarName, LPCTSTR lpVar )
    {
        ZLRegister reg;
        if (reg.Open(
            HKEY_LOCAL_MACHINE,
            _T("System\\CurrentControlSet\\Control\\Session Manager\\Environment"),
            KEY_WRITE,
            TRUE))
        {
            return reg.SetExpandSzValue(lpVarName, lpVar);
        }
        return FALSE;
    }

    inline BOOL ZLEnvironmentVar::_DelSysEnv( LPCTSTR lpVarName )
    {
        ZLRegister reg;
        if (reg.Open(
            HKEY_LOCAL_MACHINE,
            _T("System\\CurrentControlSet\\Control\\Session Manager\\Environment"),
            KEY_WRITE))
        {
            return reg.DelValue(lpVarName);
        }
        return FALSE;
    }

    inline BOOL ZLEnvironmentVar::_AddUserEnv( LPCTSTR lpVarName, LPCTSTR lpVar )
    {
        CString sSid;
        if (ZLUsid::GetCurrentUserSID(sSid))
        {
            CString sSubKey = sSid + L"\\Environment";
            ZLRegister reg;
            if (reg.Open(HKEY_USERS, sSubKey, KEY_WRITE))
            {
                return reg.SetExpandSzValue(lpVarName, lpVar);
            }
        }
        return FALSE;
    }

    inline BOOL ZLEnvironmentVar::_DelUserEnv( LPCTSTR lpVarName )
    {
        CString sSid;
        if (ZLUsid::GetCurrentUserSID(sSid))
        {
            CString sSubKey = sSid + L"\\Environment";
            ZLRegister reg;
            if (reg.Open(HKEY_USERS, sSubKey, KEY_WRITE))
            {
                return reg.DelValue(lpVarName);
            }
        }
        return FALSE;
    }
}
}
