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

namespace zl
{
namespace WinUtils
{

    class ZLTime
    {
    public:
        static FILETIME Time642FileTime(const __time64_t& tm)
        {
            FILETIME ft;
            LONGLONG ll;
            ll = tm*10000000 + 116444736000000000;
            ft.dwLowDateTime  = (DWORD)ll;
            ft.dwHighDateTime = (DWORD)(ll >> 32);
            return ft;
        }

        static __time64_t FileTime2Time64(const FILETIME& pft)
        {
            __time64_t cTime = pft.dwHighDateTime;
            cTime = (cTime<<32) + pft.dwLowDateTime;  
            return ((cTime - 116444736000000000)/10000000);
        }

        static BOOL SystemTime2FileTime(const SYSTEMTIME& syst, FILETIME& filet)
        {
            return ::SystemTimeToFileTime(&syst, &filet);
        }

        static BOOL FileTime2SystemTime(const FILETIME& filet, SYSTEMTIME& syst)
        {
            return ::FileTimeToSystemTime(&filet, &syst);
        }

        static SYSTEMTIME GetLocalTime()
        {
            SYSTEMTIME st;
            ::GetLocalTime(&st);
            return st;
        }

        static SYSTEMTIME GetSystemTime()
        {
            SYSTEMTIME st;
            ::GetSystemTime(&st);
            return st;
        }

        static BOOL Calc( LARGE_INTEGER& llTimeBegin, double& dfTimeInterval )
        {
            LARGE_INTEGER freq = {0};
            LARGE_INTEGER timeEnd = {0};
            if (llTimeBegin.QuadPart = 0
                && ::QueryPerformanceFrequency(&freq)
                && ::QueryPerformanceCounter(&timeEnd))
            {
                dfTimeInterval = double(timeEnd.QuadPart - llTimeBegin.QuadPart) / freq.QuadPart * 1000.0;
                return TRUE;
            }
            return FALSE;
        }

        static BOOL GetTime( LARGE_INTEGER& llCurrentTime )
        {
            return ::QueryPerformanceCounter(&llCurrentTime);
        }
    };

}
}
