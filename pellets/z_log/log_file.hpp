/********************************************************************
* @file      : log_file.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/27 10:21
* @brief     : 写日志到文件
* 
* 
*********************************************************************/

#ifndef _LOG_FILE_HPP_
#define _LOG_FILE_HPP_

#include <windows.h>

namespace zl
{
    ///> 日志文件最大体积
    const unsigned long ulMaxLogSize = 2000000; // 约2M

    class CLogFile
    {
    public:
        CLogFile(const char* lpFileName, BOOL bTrunk)
            : m_hFile(INVALID_HANDLE_VALUE)
            , m_FileName(lpFileName)
        {
            Open(bTrunk);
        }

        ~CLogFile()
        {
            Close();
        }

    public:
        void WriteLine(const char* lpStr, DWORD dwSize)
        {
            Write(lpStr, dwSize);
            char sFeed[] = "\n";
            Write(sFeed, sizeof(sFeed) - 1);
        }

    private:
        void Write(const char* lpStr, DWORD dwSize)
        {
            z_mutex_guard guard(m_FileMutex);
            DWORD dwBytesWritten;
            if (m_hFile != INVALID_HANDLE_VALUE)
            {
                ::WriteFile(m_hFile, lpStr, dwSize, &dwBytesWritten, NULL);
            }
        }

        void Close()
        {
            z_mutex_guard guard(m_FileMutex);
            if (m_hFile != INVALID_HANDLE_VALUE)
            {
                char sEnd[] = "[End]";
                WriteLine(sEnd, sizeof(sEnd) - 1);
                ::CloseHandle(m_hFile);
                m_hFile = INVALID_HANDLE_VALUE;
            }
        }

        BOOL Open(BOOL bTrunk)
        {
            z_mutex_guard guard(m_FileMutex);
            m_hFile = ::CreateFileA(
                m_FileName.c_str(),
                GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                NULL,
                bTrunk ? CREATE_ALWAYS : OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

            if(INVALID_HANDLE_VALUE == m_hFile)
                return FALSE;

            LARGE_INTEGER liOffset;
            liOffset.QuadPart = 0;
            ::SetFilePointer(m_hFile, liOffset.LowPart, &liOffset.HighPart, FILE_END);
            char sBegin[] = "[Begin]";
            WriteLine(sBegin, sizeof(sBegin) - 1);
            return TRUE;
        }

        ULONG GetFileSize()
        {
            z_mutex_guard guard(m_FileMutex);
            ULONG ulFileSize = 0;
            if (INVALID_HANDLE_VALUE != m_hFile)
            {
                ulFileSize = ::GetFileSize(m_hFile, NULL);
                ::CloseHandle(m_hFile);
            }
            return ulFileSize;
        }

        BOOL CheckFileSizeAndSet()
        {
            ULONG ulFileSize = 0;
            std::string strLogExFilePath;
            if (m_FileName.empty())
            {
                return FALSE;
            }
            z_mutex_guard guard(m_FileMutex);
            ulFileSize = GetFileSize();
            if (ulFileSize >= ulMaxLogSize)
            {
                strLogExFilePath = m_FileName + ".old";
                ::SetFileAttributesA(strLogExFilePath.c_str(), FILE_ATTRIBUTE_NORMAL);
                ::DeleteFileA(strLogExFilePath.c_str());
                return ::MoveFileA(m_FileName.c_str(), strLogExFilePath.c_str());
            }
            return TRUE;
        }

    private:

        std::string     m_FileName;
        HANDLE          m_hFile;

        ///> 文件操作互斥锁
        z_mutex         m_FileMutex;

    };


}

#endif