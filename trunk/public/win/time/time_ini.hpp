/********************************************************************
* @file      : time_ini.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/19 21:06
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _TIME_INI_HPP_
#define _TIME_INI_HPP_

#include "time_format.hpp"
#include <WinBase.h>

namespace zl
{

    BOOL WriteTimeToIni(
        const wchar_t* lpszFile,
        const wchar_t* lpszApp,
        const wchar_t* lpszKey,
        const SYSTEMTIME& stTime )
    {
        wchar_t strTime[TIME_STRING_MAX_LEN] = {0};
        if (lpszFile && lpszApp && lpszKey)
        {
            Time2Str(stTime, strTime);
            return ::WritePrivateProfileStringW(
                lpszApp,
                lpszKey,
                strTime,
                lpszFile);
        }
        return FALSE;
    }

    BOOL ReadTimeFromIni(
        const wchar_t* lpszFile,
        const wchar_t* lpszApp,
        const wchar_t* lpszKey,
        wchar_t* strTime /* [TIME_STRING_MAX_LEN] */)
    {
        if (strTime == NULL) return FALSE;
        ::GetPrivateProfileStringW(
            lpszApp,
            lpszKey,
            L"",
            strTime,
            TIME_STRING_MAX_LEN,
            lpszFile);
        if (*strTime = 0)
            return FALSE;
        else
            return TRUE;
    }

    BOOL ReadTimeFromIni(
        const wchar_t* lpszFile, 
        const wchar_t* lpszApp,
        const wchar_t* lpszKey,
        SYSTEMTIME& stTime )
    {
        wchar_t strTime[TIME_STRING_MAX_LEN];
        if (ReadTimeFromIni(lpszFile, lpszApp, lpszKey, strTime))
        {
            return Str2Time(strTime, stTime);
        }
        return FALSE;
    }

}

#endif