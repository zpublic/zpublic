#include "stdafx.h"
#include "zl_curl_file.h"

namespace zl
{

CWriteCurlMemFile::CWriteCurlMemFile( std::string &strWrite )
    : m_recvStream(strWrite)
{

}

CWriteCurlMemFile::~CWriteCurlMemFile()
{

}

void CWriteCurlMemFile::Write( BYTE* pbyBuf, u_int uSize )
{
    if (pbyBuf && uSize > 0)
    {
        m_recvStream.append((const char*)pbyBuf, uSize);
    }
}

size_t CWriteCurlMemFile::WriteCallBack( void *pvData, size_t nSize, size_t nCount, void *pvParam )
{
    CWriteCurlMemFile *pThis = static_cast<CWriteCurlMemFile *>(pvParam);
    DWORD nDataSize = DWORD(nSize * nCount);

    if (pThis)
        pThis->Write((LPBYTE)pvData, (DWORD)nDataSize);

    return nDataSize;
}


CWriteCurlFile::CWriteCurlFile( LPCTSTR lpszFileName )
    : m_hFile(INVALID_HANDLE_VALUE)
{
    m_hFile = ::CreateFileW(lpszFileName,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );
    assert(m_hFile != INVALID_HANDLE_VALUE);
}

CWriteCurlFile::~CWriteCurlFile()
{
    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
}

int CWriteCurlFile::Write( BYTE* pbyBuf, u_int uSize )
{
    if (m_hFile == INVALID_HANDLE_VALUE)
        return 0;

    DWORD dwRealWriteSize = 0;

    ::WriteFile(m_hFile, pbyBuf, uSize, &dwRealWriteSize, NULL);

    return dwRealWriteSize;
}

size_t CWriteCurlFile::WriteCallBack( void *pvData, size_t nSize, size_t nCount, void *pvParam )
{
    CWriteCurlFile *pThis = static_cast<CWriteCurlFile *>(pvParam);
    DWORD nDataSize = (DWORD)(nSize * nCount);

    if (pThis)
        return pThis->Write((LPBYTE)pvData, (DWORD)nDataSize);

    return nDataSize;
}

}
