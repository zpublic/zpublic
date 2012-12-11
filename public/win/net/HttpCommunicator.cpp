/************************************************************************/
/* File: HttpCommunicator.cpp                                           */
/* Date: 2005-11-29 18:25                                               */
/* Auth: PengNing                                                       */
/* Comm: Communicate using http(base on windows internet API)           */
/************************************************************************/
#include "HttpCommunicator.h"
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "WinInet.lib")

const TCHAR* const cszHttpVer           = _T("HTTP/1.1");

const TCHAR* const cszHttpGet           = _T("GET");

const TCHAR* const cszHttpPost          = _T("POST");

const TCHAR* const pcszConnection       = _T("Connection: Keep-Alive");

const TCHAR* const pcszProxyConn        = _T("Proxy-Connection: Keep-Alive");

const TCHAR* const pcszPragma           = _T("Pragma: no-cache");

//Define the default timeout in milliseconds(5 seconds)
const unsigned long culDefaultTimeout   = 5000;

/////////////////////////////////////////////////////////////
CHttpCommunicator::CHttpCommunicator()
{
    ZeroMemory(&m_stProxyInfo, sizeof(ST_PROXY_INFO));
    ZeroMemory(&m_stServerAddr, sizeof(ST_INET_ADDR));
    m_stTimeout.ulConnTimeout = culDefaultTimeout;
    m_stTimeout.ulSendTimeout = culDefaultTimeout;
    m_stTimeout.ulRecvTimeout = culDefaultTimeout;
    m_stProxyInfo.nProxyMethod  = enumPROXY_METHOD_DIRECT;
}

CHttpCommunicator::~CHttpCommunicator()
{
}

/////////////////////////////////////////////////////////////
bool CHttpCommunicator::DoHttpGet(
    IN      const TCHAR*    pszGet, 
    OUT     void*           pvRecvBuf, 
    IN OUT  DWORD&          dwRecvBufSize 
)
{
    BOOL bRetCode	= FALSE;
    bool bResult	= false;

    HINTERNET	hInetOpen	= NULL; //for InternetOpen()
    HINTERNET	hInetConnect= NULL;	//for InternetConnect()
    HINTERNET	hInetRequest= NULL;	//for HttpOpenrequest()

    //check pointer
    if (!pszGet || !pvRecvBuf)
        goto Exit0;

    //Connect
    if(!DoInternetConnect(&hInetOpen, &hInetConnect))
        goto Exit0;

    if (!hInetOpen || !hInetConnect)
        goto Exit0;

    //open a request
    hInetRequest = HttpOpenRequest(hInetConnect, 
        cszHttpGet,
        pszGet,
        cszHttpVer,
        NULL,
        NULL,
        INTERNET_FLAG_DONT_CACHE,
        0
    );
    if (NULL == hInetRequest)
        goto Exit0;

    //add HTTP request header
    if(!AddHttpRequestHeader(&hInetRequest))
        goto Exit0;

    //send a request
    if(!HttpSendRequest(hInetRequest, NULL, 0, NULL, 0))
        goto Exit0;

    //Receive data
    if(!DoHttpDataReceive(&hInetRequest, pvRecvBuf, dwRecvBufSize))
        goto Exit0;

    bResult = true;
Exit0:
    if (NULL != hInetOpen)
    {
        InternetCloseHandle(hInetOpen);
    }
    if (NULL != hInetConnect)
    {
        InternetCloseHandle(hInetConnect);
    }
    if (NULL != hInetRequest)
    {
        InternetCloseHandle(hInetRequest);
    }

    return bResult;
}

bool CHttpCommunicator::DoHttpPost(
    IN      const TCHAR*    pszPost, 
    IN      const void*     pvSendBuf, 
    IN      DWORD           dwSendBufSize, 
    OUT     void*           pvRecvBuf, 
    IN OUT  DWORD&          dwRecvBufSize 
)
{
    BOOL bRetCode	= FALSE;
    bool bResult	= false;

    HINTERNET	hInetOpen	= NULL;		//for InternetOpen()
    HINTERNET	hInetConnect= NULL;		//for InternetConnect()
    HINTERNET	hInetRequest= NULL;		//for HttpOpenrequest()

    //check pointer in
    if (!pszPost || !pvSendBuf || !pvRecvBuf)
        goto Exit0;

    if(!DoInternetConnect(&hInetOpen, &hInetConnect))
        goto Exit0;

    if (!hInetOpen || !hInetConnect)
        goto Exit0;

    //open a request
    hInetRequest = ::HttpOpenRequest(hInetConnect, 
        cszHttpPost,
        pszPost,
        cszHttpVer,
        NULL,
        0,
        INTERNET_FLAG_DONT_CACHE,
        0
    );
    if (NULL == hInetRequest)
        goto Exit0;

    //add HTTP request header
    if(!AddHttpRequestHeader(&hInetRequest))
        goto Exit0;

    //send a request
    bRetCode = ::HttpSendRequest(hInetRequest, 
        NULL, 
        0, 
        (char* )pvSendBuf, 
        dwSendBufSize
    );
    if (false == bRetCode)
        goto Exit0;

    if(!DoHttpDataReceive(&hInetRequest, pvRecvBuf, dwRecvBufSize))
        goto Exit0;

    bResult = true;
Exit0:
    if (hInetOpen)
    {
        InternetCloseHandle(hInetOpen);
    }
    if (hInetConnect)
    {
        InternetCloseHandle(hInetConnect);
    }
    if (hInetRequest)
    {
        InternetCloseHandle(hInetRequest);
    }

    return bResult;
}

bool CHttpCommunicator::DoInternetConnect(
    HINTERNET*      phInetOpen, 
    HINTERNET*      phInetConnect
)
{
    BOOL bRetCode   = FALSE;

    //Initializes an application's use of the WinINet functions
    *phInetOpen = InternetOpen(
        _T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)@^_^@"), 
        INTERNET_OPEN_TYPE_DIRECT, 
        NULL, 
        NULL,
        0
    );
    if (NULL == *phInetOpen)
        return false;

    //set proxy(custom) options if needed
    INTERNET_PROXY_INFO	stProxtInfo;
    TCHAR szProxy[cnProxyStrMaxLen]	= {0};
    ZeroMemory(&stProxtInfo, sizeof(INTERNET_PROXY_INFO));

    switch(m_stProxyInfo.nProxyMethod)
    {
    case enumPROXY_METHOD_DIRECT:
        stProxtInfo.dwAccessType    = INTERNET_OPEN_TYPE_DIRECT;
        break;

    case enumPROXY_METHOD_USE_IE:
        stProxtInfo.dwAccessType    = INTERNET_OPEN_TYPE_PRECONFIG;
        break;

    case enumPROXY_METHOD_CUSTOM:
        stProxtInfo.dwAccessType    = INTERNET_OPEN_TYPE_PROXY;
        if(enumPROXY_TYPE_HTTP == m_stProxyInfo.nProxyType)
        {
            int nRet = _sntprintf(szProxy, 
                cnProxyStrMaxLen,
                _T("http=http://%s:%d"),
                m_stProxyInfo.szProxyAddr,
                m_stProxyInfo.usProxyPort
            );
            if(nRet <= 0 || cnProxyStrMaxLen == nRet)
                return false;
        }
        else
        {
            int nRet = _sntprintf(szProxy,
                cnProxyStrMaxLen, 
                _T("socks=%s:%d"),
                m_stProxyInfo.szProxyAddr,
                m_stProxyInfo.usProxyPort
            );
            if(nRet <= 0 || cnProxyStrMaxLen == nRet)
                return false;
        }
        break;

    default:
        return false;
    }

    stProxtInfo.lpszProxy       = szProxy;
    stProxtInfo.lpszProxyBypass = NULL;

    //Set proxy option
    bRetCode = InternetSetOption(*phInetOpen,
        INTERNET_OPTION_PROXY, 
        &stProxtInfo,
        sizeof(INTERNET_PROXY_INFO)
    );
    if (FALSE == bRetCode)
        return false;

    if(enumPROXY_METHOD_CUSTOM == m_stProxyInfo.nProxyMethod)
    {
        bRetCode = InternetSetOption(*phInetOpen, 
            INTERNET_OPTION_PROXY_USERNAME,
            m_stProxyInfo.szUserName,
            (DWORD)(_tcslen(m_stProxyInfo.szProxyAddr) + 1)
        );
        if (FALSE == bRetCode)
            return false;

        bRetCode = ::InternetSetOption(*phInetOpen,
            INTERNET_OPTION_PROXY_PASSWORD,
            m_stProxyInfo.szPassword,
            (DWORD)(_tcslen(m_stProxyInfo.szPassword) + 1)
        );
        if (FALSE == bRetCode)
            return false;
    }

    //Opens an HTTP session.
    *phInetConnect = InternetConnect(*phInetOpen,
        m_stServerAddr.szAddr,
        m_stServerAddr.usPort,
        NULL,
        NULL,
        INTERNET_SERVICE_HTTP,
        0,
        0
    );
    if (NULL == *phInetConnect)
        return false;

    //set timeout option
    bRetCode = ::InternetSetOption(*phInetConnect,
        INTERNET_OPTION_CONNECT_TIMEOUT,
        &(m_stTimeout.ulConnTimeout),
        sizeof(unsigned long)
    );
    if(FALSE == bRetCode)
        return false;

    bRetCode = ::InternetSetOption(*phInetConnect,
        INTERNET_OPTION_SEND_TIMEOUT,
        &(m_stTimeout.ulSendTimeout),
        sizeof(unsigned long)
    );
    if (FALSE == bRetCode)
        return false;

    bRetCode = ::InternetSetOption(*phInetConnect,
        INTERNET_OPTION_RECEIVE_TIMEOUT, 
        &(m_stTimeout.ulRecvTimeout),
        sizeof(unsigned long)
    );
    if (FALSE == bRetCode)
        return false;

    return true;
}

bool CHttpCommunicator::DoHttpDataReceive(
    IN      HINTERNET*  phInetRequest, 
    OUT     void*       pvRecv, 
    IN OUT  DWORD&      dwSize 
)
{
    bool    bReturn         = false;
    BOOL	bRetCode        = FALSE;
    DWORD   dwRecvSize      = 0;

    //Check pointer
    if(!phInetRequest || !pvRecv)
        return false;

    char  szBackData[cnRecvBufMaxSize] = {0};
    char* pszCurrent = szBackData;

    while("I believe I can fly")
    {
        DWORD	dwBackSize = 0;
        bRetCode = ::InternetReadFile(*phInetRequest,
            pszCurrent,
            static_cast<DWORD>(sizeof(szBackData) - 
            (pszCurrent - szBackData)),
            &dwBackSize
        );

        if(!bRetCode)
        {
            return false;
        }

        //Check if all data is retrieved
        if(0 == dwBackSize)
            break;

        //Change data pointer, the continue reading data
        pszCurrent	+= dwBackSize;
        dwRecvSize	+= dwBackSize;
    }

    if(dwSize < dwRecvSize)
        return false;

    memcpy(pvRecv, szBackData, dwRecvSize);
    dwSize = dwSize;

    return true;
}

bool CHttpCommunicator::AddHttpRequestHeader(IN HINTERNET* phInetRequest)
{
    //Now, Not need, just do nothing
    //It will be completed later
    return TRUE;
}
