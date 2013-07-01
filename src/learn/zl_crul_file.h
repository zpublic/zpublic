#pragma once

#include <windows.h>
#include <string>

namespace zl
{

class CWriteCurlMemFile
{
public:
	CWriteCurlMemFile(std::string &strWrite);
	~CWriteCurlMemFile();

    void Write(BYTE* pbyBuf, u_int uSize);

    static size_t WriteCallBack(void *pvData, size_t nSize, size_t nCount, void *pvParam);

private:
    std::string &m_recvStream;
};

class CWriteCurlFile
{
public:
    CWriteCurlFile(LPCTSTR lpszFileName);
    ~CWriteCurlFile();

    int Write(BYTE* pbyBuf, u_int uSize);

    static size_t WriteCallBack(void *pvData, size_t nSize, size_t nCount, void *pvParam);

private:
    HANDLE m_hFile;
};

}
