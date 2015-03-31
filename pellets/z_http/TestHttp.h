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
        TEST_ADD(CTestHttp::test_progress);
    }

    void test_post()
    {
        ZLMemWrite m;
        TEST_ASSERT(400 == m_http.PostData(L"http://www.oschina.net//action/tweet/pub", NULL, 0, &m));
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

    class Progress : public ICurlProgress
    {
    public:
        virtual int OnProgress(double dltotal, double dlnow, double ultotal, double ulnow)
        {
            std::cout << dltotal << " " << dlnow << " " << ultotal << " " << ulnow << std::endl;
            return 0;
        }
    };

    void test_progress()
    {
        Progress p;
        ZLMemWrite m;
        {
            TEST_ASSERT(0 == m_http.DownloadMem(
                L"http://www.icbc.com.cn/icbc/html/download/EbankToolsSoftware/x64/ICBCSetupIntegration_64.msi",
                &m,
                0,
                static_cast<ICurlProgress*>(&p)));
            TEST_ASSERT(m.GetLength() != 0);
        }
        {
            ZLMemWrite m2;
            TEST_ASSERT(400 == m_http.PostData(
                L"http://www.oschina.net//action/tweet/pub",
                (unsigned char*)m.GetData(),
                m.GetLength(),
                &m2,
                0,
                static_cast<ICurlProgress*>(&p)));
            TEST_ASSERT(m2.GetLength() != 0);
        }
    }

    ZLHttpClient m_http;
};
