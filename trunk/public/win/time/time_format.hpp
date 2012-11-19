/********************************************************************
* @file      : time_format.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/19 20:40
* @brief     : time format convert
* 
* 
*********************************************************************/

#ifndef _TIME_FORMAT_HPP_
#define _TIME_FORMAT_HPP_

#include <time.h>
#include <stdio.h>

#ifndef _FILETIME_
#define _FILETIME_
typedef struct _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} 	FILETIME;
#endif

#define TIME_STRING_MAX_LEN 28

namespace zl
{

    FILETIME Time642FileTime(__time64_t tm)
    {
        FILETIME ft;
        LONGLONG ll;
        ll = tm*10000000 + 116444736000000000;
        ft.dwLowDateTime  = (DWORD)ll;
        ft.dwHighDateTime = (DWORD)(ll >> 32);
        return ft;
    }

    __time64_t FileTime2Time64(const FILETIME* pft)
    {
        __time64_t cTime = pft->dwHighDateTime;
        cTime = (cTime<<32) + pft->dwLowDateTime;  
        return ((cTime - 116444736000000000)/10000000);
    }

    void Time2Str(const SYSTEMTIME& stTime, wchar_t* strTime /* [TIME_STRING_MAX_LEN] */ )
    {
        ::wsprintfW(strTime, L"%d-%d-%d %d:%d:%d:%d %d",
            stTime.wYear, stTime.wMonth, stTime.wDay,
            stTime.wHour, stTime.wMinute, stTime.wSecond,
            stTime.wMilliseconds, stTime.wDayOfWeek);
    }

    BOOL Str2Time(const wchar_t* strTime, SYSTEMTIME& stTime)
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

}

#endif
