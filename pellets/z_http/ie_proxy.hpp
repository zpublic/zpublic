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
#include "../z_win_utils/register.hpp"


#define ZL_HTTP_IE_REGISTRY_PATH    L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"
#define ZL_HTTP_IE_PROXY_HTTP       L"http="
#define ZL_HTTP_IE_PROXY_SOCKS      L"socks="

namespace zl
{
    namespace http
    {
        class ZLIeProxy
        {
        public:
            ZLIeProxy()
            {
                m_bEnable = FALSE;
                m_bAuth = FALSE;
                m_nType = enumProxyNone;
            }

            virtual ~ZLIeProxy()
            {

            }

            void Read()
            {
                zl::WinUtils::ZLRegister regKey;
                CString strProxySetting;
                DWORD dwEnable = 0;
                CString strHost;
                CString strPort;
                enumProxyType eProxyType = enumProxyNone;

                if (!regKey.Open(HKEY_CURRENT_USER, ZL_HTTP_IE_REGISTRY_PATH))
                    goto Exit0;

                if (!regKey.Read(L"ProxyEnable", dwEnable))
                    goto Exit0;

                if (!dwEnable)
                    goto Exit0;

                if (!regKey.Read(L"ProxyServer", strProxySetting))
                    goto Exit0;

                strProxySetting.MakeLower();
                int nPos = strProxySetting.Find(ZL_HTTP_IE_PROXY_HTTP);
                if (nPos != -1)
                {
                    eProxyType = enumProxyHttp;
                    strPort = _T("80");
                    strProxySetting = strProxySetting.Mid(nPos + (int)_tcsclen(ZL_HTTP_IE_PROXY_HTTP));
                }

                if (nPos == -1)
                {
                    nPos = strProxySetting.Find(ZL_HTTP_IE_PROXY_SOCKS);
                    if (nPos == -1) goto Exit0;

                    eProxyType = enumProxySocket5;
                    strPort = _T("1080");
                    strProxySetting = strProxySetting.Mid(nPos + (int)_tcsclen(ZL_HTTP_IE_PROXY_SOCKS));
                }

                nPos = strProxySetting.Find(_T(":"));
                if (nPos != -1)
                {
                    strHost = strProxySetting.Left(nPos);
                    if (strHost.IsEmpty()) goto Exit0;

                    strPort = strProxySetting.Mid(nPos + 1);
                    if (strPort.IsEmpty()) goto Exit0;
                }

                m_nType = eProxyType;
                m_strServer = strHost;
                m_strPort = strPort;
                m_bEnable = TRUE;
Exit0:
                return;
            }

            BOOL IsEnable()
            {
                if (!m_bEnable)
                    return FALSE;

                return (m_nType != enumProxyNone);
            }

            enumProxyType GetProxyType()
            {
                return m_nType;
            }

            CString GetProxyHost()
            {
                return m_strServer;
            }

            CString GetProxyPort()
            {
                return m_strPort;
            }

            CString GetProxyUserName()
            {
                return m_strUser;
            }

            CString GetProxyPassword()
            {
                return m_strPwd;
            }

        private:
            BOOL m_bEnable;
            enumProxyType m_nType;
            CString m_strServer;
            CString m_strPort;
            BOOL m_bAuth;
            CString m_strUser;
            CString m_strPwd;
        };
    }
}
