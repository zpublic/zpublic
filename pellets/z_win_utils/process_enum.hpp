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
#include "privilege.hpp"
#include <vector>

namespace zl
{
namespace WinUtils
{
    typedef struct _ZLProcessEnumInfo
    {
        _ZLProcessEnumInfo()
        {
            dwCntUsage = 0;
            dwTh32ProcessID = 0;
            dwTh32ModuleID = 0;
            dwCntThreads = 0;
            dwTh32ParentProcessID = 0;
            dwFlags = 0;
            pulTh32DefaultHeapID = NULL;
            lpcPriClassBase = 0;
        }
        DWORD dwCntUsage;
        DWORD dwTh32ProcessID;
        DWORD dwTh32ModuleID;
        DWORD dwCntThreads;
        DWORD dwTh32ParentProcessID;
        DWORD dwFlags;
        ULONG_PTR pulTh32DefaultHeapID;
        LONG lpcPriClassBase;
        CString cstrExeFile;
    }ZLProcessEnumInfo;

    typedef std::vector<ZLProcessEnumInfo> ZLProcessEnumInfoVec;

    class ZLProcessEnum
    {
    public:
        static BOOL Enum(ZLProcessEnumInfoVec& infoVec)
        {
            infoVec.clear();
            BOOL bRet = FALSE;
            HANDLE hSnapProc = INVALID_HANDLE_VALUE;
            do
            {
                hSnapProc = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (INVALID_HANDLE_VALUE == hSnapProc)
                {
                    break;
                }

                PROCESSENTRY32 pe32 = {0};
                pe32.dwSize = sizeof(pe32);
                if (!::Process32FirstW(hSnapProc, &pe32))
                {
                    break;
                }

                do
                {
                    ZLProcessEnumInfo proInfo;
                    proInfo.dwCntUsage = pe32.cntUsage;
                    proInfo.dwTh32ProcessID = pe32.th32ProcessID;
                    proInfo.dwTh32ModuleID = pe32.th32ModuleID;
                    proInfo.dwCntThreads = pe32.cntThreads;
                    proInfo.dwTh32ParentProcessID = pe32.th32ParentProcessID;
                    proInfo.dwFlags = pe32.dwFlags;
                    proInfo.pulTh32DefaultHeapID = pe32.th32DefaultHeapID;
                    proInfo.lpcPriClassBase = pe32.pcPriClassBase;
                    proInfo.cstrExeFile = pe32.szExeFile;
                    infoVec.push_back(proInfo);
                } while (::Process32NextW(hSnapProc, &pe32));
                bRet = TRUE;
            } while (FALSE);
            if (hSnapProc != INVALID_HANDLE_VALUE)
            {
                ::CloseHandle(hSnapProc);
            }
            return bRet;
        }
    };
}
}