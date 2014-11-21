#pragma once

#include "zl_curl_file.h"

namespace zl
{

enum emCurlDownEvent
{
    CurlDownEventBegin  = 0,
    CurlDownEventEnd    = 1,
};

enum emCurlDownStatus
{
    CurlDownStatusErr   = 0,
    CurlDownStatusSuc   = 1,
};

class ICurlDownProgress
{
public:
    virtual int OnProgress(double dCurProgress, double dTotalProgress) = 0;
    virtual int OnEvent(int nEvent, int nStatus) = 0;
};

class CHttpCurl
{
public:
    static int Post(LPCTSTR szUrl, LPBYTE pBuffer, int nSize, int nConnectTime, int nSendtime, std::string &strRead);
    static int Download(LPCTSTR szUrl, LPCTSTR szOutputFileName , int nRectime, ICurlDownProgress *pProgress = NULL);

private:
    static size_t ProgressFun(void *userdata, double dltotal, double dlnow, double ultotal,	double ulnow);
};

}