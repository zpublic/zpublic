/*************************************************************************
 *                                                                       *
 *  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
 *  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
 *        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
 *       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
 *     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
 *    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
 *   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
 * !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
 *                                                                       *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/


#pragma once
#include "z_http_header.h"
#include "z_http_interface.h"


namespace zl
{
    namespace http
    {
        class ZLSimpleCurl
        {
        public:
            ZLSimpleCurl()
            {
                m_nStatusCode = 0;
                m_nTimeLimit = 0;
                m_eProxyType = enumProxyNone;
                m_pWriteCallBack = NULL;
                m_pProgressCallBack = NULL;
                m_pPostData = NULL;
                m_nPostDataLen = 0;
                m_bUseSSL = TRUE;

// 版本号大于等于0x070f05的curl库,才支持限速.
#if LIBCURL_VERSION_NUM >= 0x070f05
                m_nSpeedLimit = 0;
#endif // LIBCURL_VERSION_NUM >= 0x070f05
            }

            ~ZLSimpleCurl()
            {

            }

            BOOL SetTimeLimit(int nTimeSec)
            {
                m_nTimeLimit = nTimeSec;
                return TRUE;
            }

// 版本号大于等于0x070f05的curl库,才支持限速.
#if LIBCURL_VERSION_NUM >= 0x070f05
            int SetMaxSpeedLimit(int nSpeedLimit)
            {
                int nOldSpeed = m_nSpeedLimit;
                m_nSpeedLimit = nSpeedLimit;
                return nOldSpeed;
            }
#endif // LIBCURL_VERSION_NUM >= 0x070f05

            BOOL SetProxy(enumProxyType eProxyType, LPCTSTR szHost, LPCTSTR szPort, LPCTSTR szUserName, LPCTSTR szPassword)
            {
                m_eProxyType = eProxyType;
                m_strHost = szHost;
                m_strPort = szPort;
                m_strUserName = szUserName;
                m_strPassword = szPassword;
                return TRUE;
            }

            BOOL SetPostData(void* pBuffer, DWORD dwSize)
            {
                m_pPostData = pBuffer;
                m_nPostDataLen = dwSize;
                return TRUE;
            }

            void SetMethod(LPCTSTR szMethod)
            {
                m_strMethod = szMethod;
            }

            BOOL SetWriteCallBack(ICurlWrite *pWriteCallBack)
            {
                m_pWriteCallBack = pWriteCallBack;
                return TRUE;
            }

            BOOL SetProgressCallBack(ICurlProgress *pProgress)
            {
                m_pProgressCallBack = pProgress;
                return TRUE;
            }

            void EnableSSL(BOOL bEnableSSL)
            {
                m_bUseSSL = bEnableSSL;
            }

            BOOL Navigate(LPCTSTR szUrl)
            {
                BOOL bRet = FALSE;
                CURLcode nRetCode;
                CStringA sServerA;
                CStringA sAuth;
                char *szIp = NULL;

                CURL *pCURL = curl_easy_init();
                CStringA strUrl = ATL::CW2A(szUrl);
                nRetCode = curl_easy_setopt((CURL*)pCURL, CURLOPT_URL, (LPCSTR)strUrl);
                if (nRetCode != CURLE_OK) goto Exit0;

                if (m_eProxyType != enumProxyNone)
                {
                    sServerA.Format("%ws:%ws", m_strHost, m_strPort);
                    if (m_eProxyType == enumProxyHttp)
                        curl_easy_setopt(pCURL, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
                    else
                        curl_easy_setopt(pCURL, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);

                    curl_easy_setopt(pCURL, CURLOPT_PROXY, sServerA);

                    if (!m_strUserName.IsEmpty())
                    {
                        sAuth = CW2A(m_strUserName + _T(":") + m_strPassword);
                        curl_easy_setopt(pCURL, CURLOPT_PROXYUSERPWD, (LPCSTR)sAuth);
                    }
                }

                SetMethodInfo(pCURL);

                nRetCode = curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, WriteCallBack);
                if (nRetCode != CURLE_OK) goto Exit0;

                nRetCode = curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void*)m_pWriteCallBack);
                if (nRetCode != CURLE_OK) goto Exit0;

                nRetCode = curl_easy_setopt(pCURL, CURLOPT_PROGRESSFUNCTION, ProgressCallBack);
                if (nRetCode != CURLE_OK) goto Exit0;

                nRetCode = curl_easy_setopt(pCURL, CURLOPT_PROGRESSDATA, (void*)m_pProgressCallBack); 
                if (nRetCode != CURLE_OK) goto Exit0;

// 版本号大于等于0x070f05的curl库,才支持限速.
#if LIBCURL_VERSION_NUM >= 0x070f05
                if (m_nSpeedLimit)
                    nRetCode = curl_easy_setopt(pCURL, CURLOPT_MAX_RECV_SPEED_LARGE, (curl_off_t)m_nSpeedLimit);
                if (nRetCode != CURLE_OK) goto Exit0;
#endif // LIBCURL_VERSION_NUM >= 0x070f05

                if (m_bUseSSL)
                    nRetCode = curl_easy_setopt(pCURL, CURLOPT_SSL_VERIFYPEER, FALSE); // 对于ssl，这样设置才能成功
                if (nRetCode != CURLE_OK) goto Exit0;

                if (m_nTimeLimit)
                {
                    curl_easy_setopt(pCURL, CURLOPT_CONNECTTIMEOUT, (curl_off_t)m_nTimeLimit);
                    curl_easy_setopt(pCURL, CURLOPT_TIMEOUT, (curl_off_t)m_nTimeLimit);
                }

                nRetCode = curl_easy_setopt(pCURL, CURLOPT_FOLLOWLOCATION, 1);
                if (nRetCode != CURLE_OK) goto Exit0;

// 版本号大于等于0x071300的curl库,才支持取原始IP
#if LIBCURL_VERSION_NUM >= 0x071300
                nRetCode = curl_easy_perform(pCURL);
                curl_easy_getinfo(pCURL, CURLINFO_PRIMARY_IP, &szIp);
                if (szIp) m_strAddr = CA2W(szIp);

                if (nRetCode != CURLE_OK)
                {
                    m_nStatusCode = nRetCode;
                    goto Exit0;
                }
#endif // LIBCURL_VERSION_NUM >= 0x071300

                curl_easy_getinfo(pCURL, CURLINFO_RESPONSE_CODE, &m_nStatusCode);
                if (m_nStatusCode == 200) bRet = TRUE;
Exit0:
                if (pCURL)
                    curl_easy_cleanup(pCURL);

                return bRet;
            }

            int GetStatus()
            {
                return m_nStatusCode;
            }

// 版本号大于等于0x071300的curl库,才支持取原始IP
#if LIBCURL_VERSION_NUM >= 0x071300
            LPCTSTR GetConnectAddr()
            {
                return m_strAddr;
            }
#endif // LIBCURL_VERSION_NUM >= 0x071300

        protected:
            static size_t WriteCallBack(void *pvData, size_t nSize, size_t nCount, void *pvParam)
            {
                ICurlWrite *pWrite = (ICurlWrite*)pvParam;
                size_t nDataSize = (int)nSize * (int)nCount;
                if (pWrite)
                    nDataSize = pWrite->Write((LPBYTE)pvData, (DWORD)nDataSize);
                return nDataSize;
            }

            static size_t ProgressCallBack(void *userdata, double dltotal, double dlnow, double ultotal,	double ulnow)
            {
                size_t nReturn = 0;
                ICurlProgress *pProgress = (ICurlProgress*)userdata;
                if (pProgress)
                    nReturn = pProgress->OnProgress(dlnow, dltotal);
                return nReturn;
            }

            int SetMethodInfo(CURL *pCurl)
            {
                int nRetCode = 0;

                //default = get
                m_strMethod.MakeLower();
                if (m_strMethod == _T("post"))
                {
                    nRetCode = curl_easy_setopt(pCurl, CURLOPT_POST, TRUE);
                    if (nRetCode != CURLE_OK)		goto Exit0;

                    if (m_pPostData)
                    {
                        nRetCode = curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, m_pPostData);
                        if (nRetCode != CURLE_OK)		goto Exit0;

                        nRetCode = curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, m_nPostDataLen); 
                        if (nRetCode != CURLE_OK)		goto Exit0;
                    }
                }
                else if (m_strMethod == _T("head"))
                {
                    nRetCode = curl_easy_setopt(pCurl, CURLOPT_NOBODY, TRUE);
                    if (nRetCode != CURLE_OK)		goto Exit0;
                }
Exit0:
                return nRetCode;
            }

        private:
            CString m_strMethod;
            int m_nStatusCode;
            int m_nTimeLimit;
            void *m_pPostData;
            int m_nPostDataLen;
            enumProxyType m_eProxyType;
            CString m_strHost;
            CString m_strPort;
            CString m_strUserName;
            CString m_strPassword;
            ICurlWrite *m_pWriteCallBack;
            ICurlProgress *m_pProgressCallBack;
            BOOL m_bUseSSL; // 是否使用SSL

// 版本号大于等于0x071300的curl库,才支持取原始IP
#if LIBCURL_VERSION_NUM >= 0x071300
            CString m_strAddr;
#endif // LIBCURL_VERSION_NUM >= 0x071300

// 版本号大于等于0x070f05的curl库,才支持限速.
#if LIBCURL_VERSION_NUM >= 0x070f05
            int m_nSpeedLimit;
#endif // LIBCURL_VERSION_NUM >= 0x070f05

        };
    }
}
