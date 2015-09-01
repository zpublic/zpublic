#include "stdafx.h"
#include "test_libcurl.h"
#include "zl_curl_http.h"

void test_libcurl_post()
{
    string sRead;
    assert(0 == zl::CHttpCurl::Post(L"www.douban.com", NULL, 0, 2000, 2000, sRead));
    assert(!sRead.empty());
}

void test_libcurl_download()
{
    assert(0 == zl::CHttpCurl::Download(
        L"http://images.google.com.hk/intl/zh-CN_cn/images/logos/images_logo_lg.gif",
        L"./libcurl_test.gif",
        10000));
    assert(::PathFileExists(L"./libcurl_test.gif") == TRUE);
}

void test_libcurl()
{
    test_libcurl_post();
    test_libcurl_download();
}

