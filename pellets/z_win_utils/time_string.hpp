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

    class ZLTimeString
    {
    private:
        static const int TIME_STRING_MAX_LEN = 28;

    public:
        static void Time2Str(IN const SYSTEMTIME& stTime, OUT CString& strTime)
        {
            strTime.Format(L"%d-%d-%d %d:%d:%d:%d %d",
                stTime.wYear, stTime.wMonth, stTime.wDay,
                stTime.wHour, stTime.wMinute, stTime.wSecond,
                stTime.wMilliseconds, stTime.wDayOfWeek);
        }

        static BOOL Str2Time(IN const wchar_t* strTime, OUT SYSTEMTIME& stTime)
        {
            int nYear  = 0;
            int nMonth = 0;
            int nDay   = 0;
            int nHour  = 0;
            int nMinute = 0;
            int nSecond = 0;
            int nMilliseconds = 0;
            int nDayOfWeek = 0;
            if (::swscanf_s(strTime, L"%d-%d-%d %d:%d:%d:%d %d",
                &nYear, &nMonth, &nDay,
                &nHour, &nMinute, &nSecond,
                &nMilliseconds, &nDayOfWeek) == -1)
            {
                return FALSE;
            }
            stTime.wYear         = nYear;
            stTime.wMonth        = nMonth;
            stTime.wDayOfWeek    = nDayOfWeek;
            stTime.wDay          = nDay;
            stTime.wHour         = nHour;
            stTime.wMinute       = nMinute;
            stTime.wSecond       = nSecond;
            stTime.wMilliseconds = nMilliseconds;
            return TRUE;
        }

        static void Time2StrShort(IN const SYSTEMTIME& stTime, OUT CString& strTime)
        {
            strTime.Format(L"%d-%d-%d %d:%d:%d",
                stTime.wYear, stTime.wMonth, stTime.wDay,
                stTime.wHour, stTime.wMinute, stTime.wSecond);
        }

        static BOOL Str2TimeShort(IN const wchar_t* strTime, OUT SYSTEMTIME& stTime)
        {
            int nYear  = 0;
            int nMonth = 0;
            int nDay   = 0;
            int nHour  = 0;
            int nMinute = 0;
            int nSecond = 0;
            if (::swscanf_s(strTime, L"%d-%d-%d %d:%d:%d",
                &nYear, &nMonth, &nDay,
                &nHour, &nMinute, &nSecond) == -1)
            {
                return FALSE;
            }
            stTime.wYear         = nYear;
            stTime.wMonth        = nMonth;
            stTime.wDay          = nDay;
            stTime.wHour         = nHour;
            stTime.wMinute       = nMinute;
            stTime.wSecond       = nSecond;
            return TRUE;
        }

        static BOOL WriteTimeToIni(IN LPCTSTR lpszFile, 
            IN LPCTSTR lpszApp, 
            IN LPCTSTR lpszKey, 
            IN const SYSTEMTIME& stTime )
        {
            CString strTime;
            if (lpszFile 
                && lpszApp
                && lpszKey)
            {
                Time2Str(stTime, strTime);
                return ::WritePrivateProfileString(
                    lpszApp,
                    lpszKey,
                    strTime,
                    lpszFile);
            }
            return FALSE;
        }

        static BOOL ReadTimeFromIni(IN LPCTSTR lpszFile, 
            IN LPCTSTR lpszApp,
            IN LPCTSTR lpszKey,
            OUT SYSTEMTIME& stTime )
        {
            CString strTime;
            if (ReadTimeFromIni(lpszFile, lpszApp, lpszKey, strTime))
            {
                return Str2Time(strTime, stTime);
            }
            return FALSE;
        }

        static BOOL ReadTimeFromIni(IN LPCTSTR lpszFile,
            IN LPCTSTR lpszApp,
            IN LPCTSTR lpszKey,
            OUT CString& strTime )
        {
            ::GetPrivateProfileString(
                lpszApp,
                lpszKey,
                L"",
                strTime.GetBuffer(TIME_STRING_MAX_LEN),
                TIME_STRING_MAX_LEN,
                lpszFile);
            strTime.ReleaseBuffer();
            if (strTime.IsEmpty())
                return FALSE;
            else
                return TRUE;
        }
    };

}
}
