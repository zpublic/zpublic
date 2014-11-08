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
 * @brief 进程的模块枚举
 */

#pragma once
#include "win_utils_header.h"
#include "privilege.hpp"
#include <vector>
#include <tlhelp32.h>

namespace zl
{
namespace WinUtils
{
    typedef struct _ZLModuleEnumInfo
    {
        _ZLModuleEnumInfo()
        {
            dwTh32ModuleID = 0;
            dwGlblcntUsage = 0;
            dwProccntUsage = 0;
            byModBaseAddr = 0;
            dwModBaseSize = 0;
            hModule = NULL;
        }
        DWORD dwTh32ModuleID;               ///> 在其所属的进程上下文模块标识符。
        DWORD dwGlblcntUsage;               ///> 全局模块的使用计数。
        DWORD dwProccntUsage;               ///> 所属进程的范围内模块的使用计数。
        BYTE* byModBaseAddr;                ///> 基址的模块，在其所属的进程范围内。
        DWORD dwModBaseSize;                ///> 大小，以字节，模块。
        CString cstrModule;                 ///> 模块名。
        CString cstrExePath;                ///> 模块的全路径。
        HMODULE hModule;                    ///> 句柄
    }ZLModuleEnumInfo;

    typedef std::vector<ZLModuleEnumInfo> ZLModuleEnumInfoVec;
    /**
     * @brief 进程模块枚举
     */
    class ZLProcessModule
    {
    public:
        /**
         * @brief 枚举指定进程的模块
         * @param[in] dwPid   进程ID
         * @param[in] InfoVec 存放进程信息的容器
         * @return 成功返回TRUE，失败返回FALSE
         */
        static BOOL Enum(DWORD dwPid, ZLModuleEnumInfoVec& InfoVec)
        {
            InfoVec.clear();
            BOOL bRet = FALSE;
            HANDLE hSnapMod = INVALID_HANDLE_VALUE;
            do 
            {
                hSnapMod = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
                if (INVALID_HANDLE_VALUE == hSnapMod)
                {
                    break;
                }

                MODULEENTRY32W me32 = {0};
                me32.dwSize = sizeof(me32);
                if (!::Module32FirstW(hSnapMod, &me32))
                {
                    break;
                }

                do
                {
                    ZLModuleEnumInfo modeInfo;
                    modeInfo.cstrExePath = me32.szExePath;
                    modeInfo.cstrModule = me32.szModule;
                    modeInfo.dwGlblcntUsage = me32.GlblcntUsage;
                    modeInfo.dwProccntUsage = me32.ProccntUsage;
                    modeInfo.dwModBaseSize = me32.modBaseSize;
                    modeInfo.byModBaseAddr = me32.modBaseAddr;
                    modeInfo.dwTh32ModuleID = me32.th32ModuleID;
                    modeInfo.hModule = me32.hModule;
                    InfoVec.push_back(modeInfo);
                    ::ZeroMemory(&me32, sizeof(me32));
                    me32.dwSize = sizeof(me32);
                } while (::Module32NextW(hSnapMod, &me32));
                bRet = TRUE;
            } while (FALSE);
            if (hSnapMod != INVALID_HANDLE_VALUE)
            {
                ::CloseHandle(hSnapMod);
            }
            return bRet;
        }
    };
}
}