#include "stdafx.h"
#include "zl_curl_http.h"
#include "zl_curl_file.h"

namespace zl
{

int CHttpCurl::Post(LPCTSTR szUrl, LPBYTE pBuffer, int nSize, int nConnectTime, int nSendtime, std::string &strRead)
{
    int nRetCode = CURLE_FAILED_INIT;
    int nStatusCode = 0;
    std::string strURL = CW2A(szUrl);
    struct curl_slist *headers = NULL;
    CWriteCurlMemFile memfile(strRead);

    CURL *pCurl = curl_easy_init();
    if (pCurl == NULL) goto Exit0;

    char* cpszHeader = "Content-Type: application/octet-stream";
    headers = curl_slist_append (headers, cpszHeader);
    headers = curl_slist_append (headers, ("User-Agent: Mozilla/4.0"));

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_URL, strURL.c_str());
    if (nRetCode != CURLE_OK)
        goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_POST, TRUE);
    if (nRetCode != CURLE_OK)
        goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, pBuffer);
    if (nRetCode != CURLE_OK)
        goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, nSize); 
    if (nRetCode != CURLE_OK)
        goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);
    if (nRetCode != CURLE_OK)
        goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, nSendtime);
    if (nRetCode != CURLE_OK)
        goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, nConnectTime);
    if (nRetCode != CURLE_OK)
        goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CWriteCurlMemFile::WriteCallBack);
    if (nRetCode != CURLE_OK)
        goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &memfile);
    if (nRetCode != CURLE_OK)
        goto Exit0;

    nRetCode = curl_easy_perform(pCurl);
    if (nRetCode != CURLE_OK)
        goto Exit0;

    curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &nStatusCode);
    if (nStatusCode != 200) nRetCode = nStatusCode;

Exit0:
    if (headers)
        curl_slist_free_all(headers);

    if (pCurl)
        curl_easy_cleanup(pCurl);

    return nRetCode;
}

int CHttpCurl::Download(LPCTSTR szUrl, LPCTSTR szOutputFileName , int nRectime, ICurlDownProgress *pProgress/* = NULL*/)
{
    int nRetCode = CURLE_FAILED_INIT;
    int nStatusCode = 0;
    std::string strURL = CW2A(szUrl, CP_UTF8);
    CWriteCurlFile filewrite(szOutputFileName);

    CURL *pCurl = curl_easy_init();
    if (pCurl == NULL) goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_POST, 0L);
    if (nRetCode != CURLE_OK)   goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_URL, strURL.c_str());
    if (nRetCode != CURLE_OK)   goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_FORBID_REUSE, 1);
    if (nRetCode != CURLE_OK)   goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, nRectime);
    if (nRetCode != CURLE_OK)   goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 5000);
    if (nRetCode != CURLE_OK)   goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CWriteCurlFile::WriteCallBack);
    if (nRetCode != CURLE_OK)   goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &filewrite);
    if (nRetCode != CURLE_OK)   goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    if (nRetCode != CURLE_OK)   goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, ProgressFun);
    if (nRetCode != CURLE_OK) goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, (void*)pProgress); 
    if (nRetCode != CURLE_OK) goto Exit0;

    nRetCode = curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0); 
    if (nRetCode != CURLE_OK) goto Exit0;

    if (pProgress) pProgress->OnEvent(CurlDownEventBegin, CurlDownStatusSuc);
    nRetCode = curl_easy_perform(pCurl);
    if (nRetCode != CURLE_OK) goto Exit0;

    curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &nStatusCode);
    if (nStatusCode != 200) 
        nRetCode = nStatusCode;

Exit0:
    if (pCurl)
        curl_easy_cleanup(pCurl);
    if (nStatusCode != CURLE_OK)
    {
        if (pProgress) pProgress->OnEvent(CurlDownEventEnd, CurlDownStatusErr);
    } 
    else
    {
        if (pProgress) pProgress->OnEvent(CurlDownEventEnd, CurlDownStatusSuc);
    }
    return nRetCode;
}

size_t CHttpCurl::ProgressFun(void *userdata, double dltotal, double dlnow, double ultotal,	double ulnow)
{
    size_t nReturn = 0;
    ICurlDownProgress *pProgress = (ICurlDownProgress*)userdata;
    if (pProgress)
    {
        if (dltotal < 0.00001)
            dlnow = 0.0f;

        nReturn = pProgress->OnProgress(dlnow, dltotal);
    }

    return nReturn;
}

}