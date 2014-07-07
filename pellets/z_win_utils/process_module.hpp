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
#include <vector>
#include "privilege.hpp"
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
            dwTh32ProcessID = 0;
            dwGlblcntUsage = 0;
            dwProccntUsage = 0;
            byModBaseAddr = 0;
            dwModBaseSize = 0;
            hModule = NULL;
        }
        DWORD dwTh32ModuleID;
        DWORD dwTh32ProcessID;
        DWORD dwGlblcntUsage;
        DWORD dwProccntUsage;
        BYTE* byModBaseAddr;
        DWORD dwModBaseSize;
        CString cstrModule;
        CString cstrExePath;
        HMODULE hModule;
    }ZLModuleEnumInfo;

    typedef std::vector<ZLModuleEnumInfo> ZLModuleEnumInfoVec;

    class ZLProcessModule
    {
    public:
        ZLProcessModule() {}
        ~ZLProcessModule() {}

    public:
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
                    modeInfo.dwTh32ProcessID = me32.th32ProcessID;
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