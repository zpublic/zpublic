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
#include "file_write.hpp"
#include "mem_write.hpp"
#include "simple_curl.hpp"
#include "ie_proxy.hpp"
#include "../thread_sync/z_t_event.hpp"


namespace zl
{
    namespace http
    {
        class ZLStopHttpWriteWrap : public ICurlWrite
        {
        public:
            ZLStopHttpWriteWrap(zl::ThreadSync::CEvent &stopEvent, ICurlWrite *pWrite) : m_stop(stopEvent), m_pWrite(pWrite)
            {
            }

            virtual ~ZLStopHttpWriteWrap()
            {

            }

            virtual int Write(BYTE *pData, int nLength)
            {
                int nReturn = -1;

                if (m_stop.Wait(0) != WAIT_TIMEOUT)
                    goto Exit0;

                if (m_pWrite)
                    nReturn = m_pWrite->Write(pData, nLength);

Exit0:
                return nReturn;
            }

            virtual const BYTE* GetData()
            {
                if (m_pWrite)
                    return m_pWrite->GetData();

                return NULL;
            }

            virtual int GetLength()
            {
                if (m_pWrite)
                    return m_pWrite->GetLength();

                return 0;
            }

            zl::ThreadSync::CEvent &m_stop;
            ICurlWrite *m_pWrite;
        };

        class ZLStopHttpProgress : public ICurlProgress
        {
        public:
            ZLStopHttpProgress(zl::ThreadSync::CEvent &stopEvent, ICurlProgress *pProgress = NULL) : m_stop(stopEvent), m_pProgress(pProgress)
            {

            }

            virtual int OnProgress(double dCurrentSize, double dTotalSize)
            {
                if (m_stop.Wait(0) != WAIT_TIMEOUT)
                    return -1;

                if (m_pProgress)
                    return m_pProgress->OnProgress(dCurrentSize, dTotalSize);

                return 0;
            }

            zl::ThreadSync::CEvent &m_stop;
            ICurlProgress *m_pProgress;
        };

        class ZLHttpClient
        {
        public:
            ZLHttpClient()
            {
                m_stop.Create();
            }

            ~ZLHttpClient()
            {

            }

            int DownloadFile(LPCTSTR szUrl, LPCTSTR szLocalFile, int nTimeLimit = 0)
            {
                int nReturn = 0;
                ZLFileWrite fileWrite(szLocalFile);
                ZLStopHttpWriteWrap writeWrap(m_stop, &fileWrite);
                ZLStopHttpProgress progressWrap(m_stop);
                ZLSimpleCurl curl;

                SetProxy(curl);
                curl.SetWriteCallBack(&writeWrap);
                curl.SetProgressCallBack(&progressWrap);

                if (nTimeLimit)
                    curl.SetTimeLimit(nTimeLimit);

                if (!curl.Navigate(szUrl))
                    nReturn = curl.GetStatus();

                return nReturn;
            }
            int DownloadMem(LPCTSTR szUrl, ZLMemWrite *pMem, int nTimeLimit = 0)
            {
                int nReturn = 0;
                ZLStopHttpWriteWrap writeWrap(m_stop, pMem);
                ZLStopHttpProgress progressWrap(m_stop);
                ZLSimpleCurl curl;

                SetProxy(curl);
                curl.SetWriteCallBack(&writeWrap);
                curl.SetProgressCallBack(&progressWrap);

                if (nTimeLimit)
                    curl.SetTimeLimit(nTimeLimit);

                if (!curl.Navigate(szUrl))
                    nReturn = curl.GetStatus();

                return nReturn;
            }

            int PostData(LPCTSTR szUrl, unsigned char *pData, DWORD dwLength, ZLMemWrite *pMem, int nTimeLimit = 0)
            {
                int nReturn = 0;
                ZLStopHttpWriteWrap writeWrap(m_stop, pMem);
                ZLStopHttpProgress progressWrap(m_stop);
                ZLSimpleCurl curl;

                SetProxy(curl);
                curl.SetWriteCallBack(&writeWrap);
                curl.SetProgressCallBack(&progressWrap);
                curl.SetPostData(pData, dwLength);
                curl.SetMethod(_T("post"));

                if (nTimeLimit)
                    curl.SetTimeLimit(nTimeLimit);

                if (!curl.Navigate(szUrl))
                    nReturn = curl.GetStatus();

                return nReturn;
            }

            void StopAllHttpRequest()
            {
                m_stop.Set();
            }

        protected:
            void SetProxy(ZLSimpleCurl &curl)
            {
                ZLIeProxy proxy;
                proxy.Read();

                if (proxy.IsEnable())
                {
                    curl.SetProxy(proxy.GetProxyType(), 
                        proxy.GetProxyHost(), 
                        proxy.GetProxyPort(), 
                        proxy.GetProxyUserName(), 
                        proxy.GetProxyPassword());
                }
            }

        private:
            zl::ThreadSync::CEvent m_stop;
        };
    }
}

