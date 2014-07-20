#pragma once

#include "def.h"
#include "z_http/z_http.h"
#include <string>
#include "z_win_utils/file_info.hpp"

using namespace zl::http;
using namespace zl::WinUtils;

class CTestHttp : public Suite
{
public:
    ~CTestHttp(void);
    CTestHttp::CTestHttp(void)
    {
        TEST_ADD(CTestHttp::test_post);
        TEST_ADD(CTestHttp::test_mem_write);
        TEST_ADD(CTestHttp::test_file_write);
    }

    void test_post()
    {
        ZLMemWrite m;
        TEST_ASSERT(400 == m_http.PostData(L"www.douban.com", NULL, 0, &m));
        TEST_ASSERT(m.GetLength() != 0);
    }

    void test_mem_write()
    {
        ZLMemWrite m;
        TEST_ASSERT(0 == m_http.DownloadMem(L"http://www.laorouji.com/teapot/data.json", &m));
        TEST_ASSERT(m.GetLength() != 0);
    }

    void test_file_write()
    {
        LPCWSTR lpFile = L"c:\\zpublic_http_test.json";
        ZLMemWrite m;
        ::DeleteFile(lpFile);
        TEST_ASSERT(0 == m_http.DownloadFile(L"http://www.laorouji.com/teapot/data.json", lpFile));
        TEST_ASSERT(::PathFileExists(lpFile));
        LONGLONG s = 0;
        TEST_ASSERT(ZLFileInfo::GetFileSize(lpFile, s));
        TEST_ASSERT(s > 0);
        TEST_ASSERT(::DeleteFile(lpFile));
    }

    ZLHttpClient m_http;
};
