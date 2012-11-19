/********************************************************************
* @file      : file_time.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/19 20:45
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _FILE_TIME_HPP
#define _FILE_TIME_HPP

#include "time_format.hpp"
#include <fileapi.h>

namespace zl
{

    BOOL GetFileTimeInfo(
        LPCWSTR pszFileName,
        __time64_t* ptmCreate,
        __time64_t* ptmAccess,
        __time64_t* ptmWrite)
    {
        HANDLE hFile = ::CreateFileW(
            pszFileName,
            GENERIC_READ, 
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
            NULL, 
            OPEN_EXISTING, 
            FILE_ATTRIBUTE_NORMAL, 
            NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            FILETIME ftCreate = {0};
            FILETIME ftAccess = {0};
            FILETIME ftWrite  = {0};
            BOOL bResult = ::GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
            ::CloseHandle(hFile);
            if (bResult)
            {
                if (ptmCreate) 
                {
                    *ptmCreate = FileTime2Time64(&ftCreate);
                }
                if (ptmAccess)
                {
                    *ptmAccess = FileTime2Time64(&ftAccess);
                }
                if (ptmWrite)
                {
                    *ptmWrite =  FileTime2Time64(&ftWrite);
                }
                return TRUE;
            }
        }
        return FALSE;
    }

}

#endif