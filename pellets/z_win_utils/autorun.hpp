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
        enum UserType { ALL_USER, CURRENT_USER };
        
        /**
         * @brief 注册表run中添加一个启动项
         * @param[in] t         指定当前用户或All user
         * @param[in] lpValName 值名
         * @param[in] lpVal     值内容
         * @param[in] bWow64    64位重定向
         * @return 成功返回TRUE, 失败返回FALSE
         */
        static BOOL AddRegRun(UserType t, LPCTSTR lpValName, LPCTSTR lpVal, BOOL bWow64 = TRUE);

        /**
         * @brief 删除注册表run下指定的启动项
         * @param[in] t         指定当前用户或All user
         * @param[in] lpValName 值名
         * @param[in] bWow64    64位重定向
         * @return 成功返回TRUE, 失败返回FALSE
         */
        static BOOL DelRegRun(UserType t, LPCTSTR lpValName, BOOL bWow64 = TRUE);
        
        /**
         * @brief 注册表runonce中添加一个启动项
         * @see AddRegRun
         */
        static BOOL AddRegRunOnce(UserType t, LPCTSTR lpValName, LPCTSTR lpVal, BOOL bWow64 = TRUE);

        /**
         * @brief 删除注册表runonce下指定的启动项
         * @see DelRegRun
         */
        static BOOL DelRegRunOnce(UserType t, LPCTSTR lpValName, BOOL bWow64 = TRUE);

    private:
        static BOOL _SetExpandSzValueAllUser(LPCTSTR lpSubKey, LPCTSTR lpValName, LPCTSTR lpVal, BOOL bWow64);
        static BOOL _DelValueAllUser(LPCTSTR lpSubKey ,LPCTSTR lpValName, BOOL bWow64);
        static BOOL _SetExpandSzValueCurrentUser(LPCTSTR lpSubKey,LPCTSTR lpValName,LPCTSTR lpVal,BOOL bWow64);
        static BOOL _DelValueCurrentUser(LPCTSTR lpSubKey, LPCTSTR lpValName, BOOL bWow64);
    };

    inline BOOL ZLAutorun::AddRegRun( UserType t, LPCTSTR lpValName, LPCTSTR lpVal, BOOL bWow64)
    {
        if (NULL == lpValName || NULL == lpVal)
            return FALSE;

        LPCTSTR lpSubKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
        switch (t)
        {
        case ALL_USER:
            return _SetExpandSzValueAllUser(lpSubKey, lpValName, lpVal, bWow64);
        case CURRENT_USER:
            return _SetExpandSzValueCurrentUser(lpSubKey, lpValName, lpVal, bWow64);
        }
        return FALSE;
    }

    inline BOOL ZLAutorun::DelRegRun( UserType t, LPCTSTR lpValName, BOOL bWow64 )
    {
        if (NULL == lpValName)
            return FALSE;

        LPCTSTR lpSubKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
        switch (t)
        {
        case ALL_USER:
            return _DelValueAllUser(lpSubKey, lpValName, bWow64);
        case CURRENT_USER:
            return _DelValueCurrentUser(lpSubKey, lpValName, bWow64);
        }
        return FALSE;
    }

    inline BOOL ZLAutorun::AddRegRunOnce( UserType t, LPCTSTR lpValName, LPCTSTR lpVal, BOOL bWow64)
    {
        if (NULL == lpValName || NULL == lpVal)
            return FALSE;

        LPCTSTR lpKeyBase = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Runonce");
        switch (t)
        {
        case ALL_USER:
            return _SetExpandSzValueAllUser(lpKeyBase, lpValName, lpVal, bWow64);
        case CURRENT_USER:
            return _SetExpandSzValueCurrentUser(lpKeyBase, lpValName, lpVal, bWow64);
        }
        return FALSE;
    }

    inline BOOL ZLAutorun::DelRegRunOnce( UserType t, LPCTSTR lpValName, BOOL bWow64 /*= TRUE*/ )
    {
        if (NULL == lpValName)
            return FALSE;

        LPCTSTR lpKeyBase = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Runonce");
        switch (t)
        {
        case ALL_USER:
            return _DelValueAllUser(lpKeyBase, lpValName, bWow64);
        case CURRENT_USER:
            return _DelValueCurrentUser(lpKeyBase, lpValName, bWow64);
        }
        return FALSE;
    }

    inline BOOL ZLAutorun::_SetExpandSzValueAllUser( LPCTSTR lpSubKey, LPCTSTR lpValName, LPCTSTR lpVal, BOOL bWow64 )
    {
        ZLRegister reg;
        if (reg.Open(HKEY_LOCAL_MACHINE, lpSubKey, bWow64 ? KEY_WRITE : KEY_WOW64_64KEY | KEY_WRITE, TRUE))
        {
            return reg.SetExpandSzValue(lpValName, lpVal);
        }
        return FALSE;
    }

    inline BOOL ZLAutorun::_DelValueAllUser( LPCTSTR lpSubKey ,LPCTSTR lpValName, BOOL bWow64 )
    {
        ZLRegister reg;
        if (reg.Open(HKEY_LOCAL_MACHINE, lpSubKey, bWow64 ? KEY_WRITE : KEY_WOW64_64KEY | KEY_WRITE))
        {
            return reg.DelValue(lpValName);
        }
        return FALSE;
    }

    inline BOOL ZLAutorun::_SetExpandSzValueCurrentUser( LPCTSTR lpSubKey, LPCTSTR lpValName, LPCTSTR lpVal, BOOL bWow64 )
    {
        CString sUserSid;
        if (ZLUsid::GetCurrentUserSID(sUserSid))
        {
            CString sSubKey;
            sSubKey.Format(_T("%s\\%s"), sUserSid, lpSubKey);

            ZLRegister reg;
            if (reg.Open(HKEY_USERS, sSubKey, bWow64 ? KEY_WRITE : KEY_WOW64_64KEY | KEY_WRITE, TRUE))
            {
                return reg.SetExpandSzValue(lpValName, lpVal);
            }
        }
        return FALSE;
    }

    inline BOOL ZLAutorun::_DelValueCurrentUser( LPCTSTR lpSubKey, LPCTSTR lpValName, BOOL bWow64 )
    {
        CString sUserSid;
        if (ZLUsid::GetCurrentUserSID(sUserSid))
        {
            CString sSubKey;
            sSubKey.Format(_T("%s\\%s"), sUserSid, lpSubKey);

            ZLRegister reg;
            if (reg.Open(HKEY_USERS, sSubKey, bWow64 ? KEY_WRITE : KEY_WOW64_64KEY | KEY_WRITE))
            {
                return reg.DelValue(lpValName);
            }
        }
        return FALSE;
    }
}
}
