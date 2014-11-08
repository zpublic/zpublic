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
 * @brief 进程枚举
 */


#pragma once
#include "win_utils_header.h"
#include "privilege.hpp"
#include "process.hpp"
#include "path.hpp"
#include <vector>

namespace zl
{
namespace WinUtils
{
    typedef struct _ZLProcessEnumInfo
    {
        _ZLProcessEnumInfo()
        {
            dwTh32ProcessID = 0;
            dwCntThreads = 0;
            dwTh32ParentProcessID = 0;
            lpcPriClassBase = 0;
        }
        DWORD dwTh32ProcessID;                  ///> 进程ID
        DWORD dwCntThreads;                     ///> 此进程开启的线程计数
        DWORD dwTh32ParentProcessID;            ///> 父进程的ID
        LONG lpcPriClassBase;                   ///> 线程优先权
        CString cstrExeFile;                    ///> 进程全路径
    }ZLProcessEnumInfo;

    typedef std::vector<ZLProcessEnumInfo> ZLProcessEnumInfoVec;
    /**
     * @brief 进程枚举
     */
    class ZLProcessEnum
    {
    public:
        /**
         * @brief 枚举所有进程
         * @param[in,out] infoVec 存放所有进程信息的容器
         * @return 成功返回TRUE，失败返回FALSE
         */
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
                    proInfo.dwTh32ProcessID = pe32.th32ProcessID;
                    proInfo.dwCntThreads = pe32.cntThreads;
                    proInfo.dwTh32ParentProcessID = pe32.th32ParentProcessID;
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

        /**
         * @brief 判断指定进程是否存在
         * @param[in] lpFileName 进程文件名
         * @return 存在返回TRUE，不存在返回FALSE，枚举进程失败返回FALSE
         */
        static BOOL IsProcExist(LPCWSTR lpFileName)
        {
            BOOL bExist = FALSE;
            ZLProcessEnumInfoVec vecProc;
            ZLProcessEnum::Enum(vecProc);
            for (ZLProcessEnumInfoVec::const_iterator it = vecProc.begin();
                it != vecProc.end();
                ++it)
            {
                if (ZLPath::PathToFileName(it->cstrExeFile).CompareNoCase(lpFileName) == 0)
                {
                    bExist = TRUE;
                    break;
                }
            }
            return bExist;
        }
    };
}
}