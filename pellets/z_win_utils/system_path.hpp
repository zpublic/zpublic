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
#pragma once
#include "win_utils_header.h"
#include "path.hpp"
#include "wow64.hpp"
#include <shlobj.h>

namespace zl
{
namespace WinUtils
{
     /**
     * @brief 获取系统相关的目录
     * @note 接口列表如下:
     *      GetSystemDir                    windows\system32
     *      GetWindowsDir                   windows
     *      GetCommonAppDataDir             All Users\Application Data
     *      GetCommonTempDir                All Users\Templates
     *      GetAppDataDir                   <user name>\Application Data
     *      GetTempDir                      <user name>\Templates
     *      GetProgramFileDir               Program Files
     */

    class ZLSystemPath
    {
    public:
        ZLSystemPath() {}
        ~ZLSystemPath() {}

    public:
        static CString GetSystemDir()
        {
            CString cstrRet;
            int nLen = ::GetSystemDirectory(NULL, 0);
            if (nLen > 0)
            {
                ::GetSystemDirectory(cstrRet.GetBuffer(nLen + 1), nLen);
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            return cstrRet;
        }

        static CString GetWindowsDir()
        {
            CString cstrRet;
            int nLen = ::GetWindowsDirectory(NULL, 0);
            if (nLen > 0)
            {
                ::GetWindowsDirectory(cstrRet.GetBuffer(nLen + 1), nLen);
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            return cstrRet;
        }

        static CString GetCommonAppDataDir()
        {
            CString cstrRet;
            ::SHGetSpecialFolderPath(NULL,
                cstrRet.GetBuffer(MAX_PATH + 1),
                CSIDL_COMMON_APPDATA,
                FALSE);
            cstrRet.ReleaseBuffer();
            ZLPath::PathAddBackslash(cstrRet);
            return cstrRet;
        }

        static CString GetAppDataDir(HANDLE hToken = NULL)
        {
            CString cstrRet;
            if (hToken != NULL)
            {
                ::SHGetFolderPath(NULL,
                    CSIDL_APPDATA,
                    hToken,
                    SHGFP_TYPE_CURRENT,
                    cstrRet.GetBuffer(MAX_PATH + 1));
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            else
            {
                ::SHGetSpecialFolderPath(NULL,
                    cstrRet.GetBuffer(MAX_PATH + 1),
                    CSIDL_APPDATA,
                    FALSE);
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            return cstrRet;
        }

        static CString GetCommonTempDir()
        {
            CString cstrRet;
            ::SHGetSpecialFolderPath(NULL,
                cstrRet.GetBuffer(MAX_PATH + 1),
                CSIDL_COMMON_TEMPLATES,
                FALSE);
            cstrRet.ReleaseBuffer();
            ZLPath::PathAddBackslash(cstrRet);
            return cstrRet;
        }

        static CString GetTempDir(HANDLE hToken = NULL)
        {
            CString cstrRet;
            if (hToken != NULL)
            {
                ::SHGetFolderPath(NULL,
                    CSIDL_TEMPLATES,
                    hToken,
                    SHGFP_TYPE_CURRENT,
                    cstrRet.GetBuffer(MAX_PATH + 1));
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            else
            {
                ::SHGetSpecialFolderPath(NULL,
                    cstrRet.GetBuffer(MAX_PATH + 1),
                    CSIDL_TEMPLATES,
                    FALSE);
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            return cstrRet;
        }

        static CString GetProgramFileDir()
        {
            CString cstrRet;
            ::SHGetSpecialFolderPath(NULL,
                cstrRet.GetBuffer(MAX_PATH + 1),
                CSIDL_PROGRAM_FILES,
                FALSE);
            cstrRet.ReleaseBuffer();
            ZLPath::PathAddBackslash(cstrRet);
            return cstrRet;
        }
    };

}
}