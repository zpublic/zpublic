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
 * @brief 系统相关路径操作
 */


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
    /**
     * @brief 获取常见的系统相关路径
     */
    class ZLSystemPath
    {
    public:
        ZLSystemPath() {}
        ~ZLSystemPath() {}

    public:
        /**
         * @brief 获取系统目录
         * @return 成功返回系统目录，失败返回空串
         * @see GetSystemDirectory
         */
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
        /**
         * @brief 获取Windows目录
         * @return 成功返回Windows目录，失败返回空串
         * @see GetWindowsDirectory
         */
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
        /**
         * @brief 获取CommonAppData目录
         * @return 成功返回CommonAppData目录，失败返回空串
         * @see SHGetSpecialFolderPath
         */
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
        /**
         * @brief 获取AppData目录
         * @return 成功返回AppData目录，失败返回空串
         * @see SHGetFolderPath
         */
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
        /**
         * @brief 获取CommonTemp目录
         * @return 成功返回CommonTemp目录，失败返回空串
         * @see SHGetSpecialFolderPath
         */
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
        /**
         * @brief 获取Temp目录
         * @return 成功返回Temp目录，失败返回空串
         * @see SHGetFolderPath
         */
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
        /**
         * @brief 获取ProgramFile目录
         * @return 成功返回ProgramFile目录，失败返回空串
         * @see SHGetFolderPath
         */
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