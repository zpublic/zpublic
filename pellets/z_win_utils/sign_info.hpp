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

/**
 * @file
 * @brief 签名信息相关
 */


#pragma once
#pragma comment(lib, "crypt32.lib")

#include <wincrypt.h>
#include <wintrust.h>

namespace zl
{
namespace WinUtils
{

    /**
     * @brief 数字签名信息相关操作
     */
    class ZLSignInfo
    {
    public:

        /**
         * @brief 判断数字签名是否有时间戳
         * @ param[in] 文件路径
         * @ return 有返回TRUE, 否则返回FALSE
         * @see http://support.microsoft.com/kb/323809/zh-cn
         */
        static BOOL IsDigitalSignatureHasTimestamp( LPCWSTR lpFilePath )
        {
            bool bRet = false;
            HCERTSTORE hStore = NULL;
            HCRYPTMSG hMsg = NULL; 
            PCCERT_CONTEXT pCertContext = NULL;
            BOOL fResult;
            DWORD dwEncoding, dwContentType, dwFormatType;
            PCMSG_SIGNER_INFO pSignerInfo = NULL;
            PCMSG_SIGNER_INFO pCounterSignerInfo = NULL;
            DWORD dwSignerInfo;
            CERT_INFO CertInfo;
            SYSTEMTIME st;

            __try
            {
                // Get message handle and store handle from the signed file.
                fResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
                    lpFilePath,
                    CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                    CERT_QUERY_FORMAT_FLAG_BINARY,
                    0,
                    &dwEncoding,
                    &dwContentType,
                    &dwFormatType,
                    &hStore,
                    &hMsg,
                    NULL);
                if (!fResult)
                {
                    __leave;
                }

                // Get signer information size.
                fResult = CryptMsgGetParam(hMsg, 
                    CMSG_SIGNER_INFO_PARAM, 
                    0, 
                    NULL, 
                    &dwSignerInfo);
                if (!fResult)
                {
                    __leave;
                }
                // Allocate memory for signer information.
                pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);
                if (!pSignerInfo)
                {
                    __leave;
                }
                // Get Signer Information.
                fResult = CryptMsgGetParam(hMsg, 
                    CMSG_SIGNER_INFO_PARAM, 
                    0, 
                    (PVOID)pSignerInfo, 
                    &dwSignerInfo);
                if (!fResult)
                {
                    __leave;
                }

                // Search for the signer certificate in the temporary 
                // certificate store.
                CertInfo.Issuer = pSignerInfo->Issuer;
                CertInfo.SerialNumber = pSignerInfo->SerialNumber;
                pCertContext = CertFindCertificateInStore(hStore,
                    X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                    0,
                    CERT_FIND_SUBJECT_CERT,
                    (PVOID)&CertInfo,
                    NULL);
                if (!pCertContext)
                {
                    __leave;
                }

                // Get the timestamp certificate signerinfo structure.
                if (_GetTimeStampSignerInfo(pSignerInfo, &pCounterSignerInfo))
                {
                    // Search for Timestamp certificate in the temporary
                    // certificate store.
                    CertInfo.Issuer = pCounterSignerInfo->Issuer;
                    CertInfo.SerialNumber = pCounterSignerInfo->SerialNumber;
                    pCertContext = CertFindCertificateInStore(hStore,
                        X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                        0,
                        CERT_FIND_SUBJECT_CERT,
                        (PVOID)&CertInfo,
                        NULL);
                    if (!pCertContext)
                    {
                        __leave;
                    }

                    // Find Date of timestamp.
                    if (_GetDateOfTimeStamp(pCounterSignerInfo, &st))
                    {
                        if (st.wYear > 2012 && st.wYear < 2048)
                        {
                            bRet = true;
                        }
                    }
                }
            }
            __finally
            {
                if (pSignerInfo != NULL) LocalFree(pSignerInfo);
                if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
                if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
                if (hStore != NULL) CertCloseStore(hStore, 0);
                if (hMsg != NULL) CryptMsgClose(hMsg);
            }
            return bRet;
        }

    private:
        static BOOL _GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo, PCMSG_SIGNER_INFO *pCounterSignerInfo)
        {   
            PCCERT_CONTEXT pCertContext = NULL;
            BOOL fReturn = FALSE;
            BOOL fResult;
            DWORD dwSize;

            __try
            {
                *pCounterSignerInfo = NULL;

                // Loop through unathenticated attributes for
                // szOID_RSA_counterSign OID.
                for (DWORD n = 0; n < pSignerInfo->UnauthAttrs.cAttr; n++)
                {
                    if (lstrcmpA(pSignerInfo->UnauthAttrs.rgAttr[n].pszObjId, 
                        szOID_RSA_counterSign) == 0)
                    {
                        // Get size of CMSG_SIGNER_INFO structure.
                        fResult = CryptDecodeObject(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                            PKCS7_SIGNER_INFO,
                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                            0,
                            NULL,
                            &dwSize);
                        if (!fResult)
                        {
                            __leave;
                        }

                        // Allocate memory for CMSG_SIGNER_INFO.
                        *pCounterSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSize);
                        if (!*pCounterSignerInfo)
                        {
                            __leave;
                        }

                        // Decode and get CMSG_SIGNER_INFO structure
                        // for timestamp certificate.
                        fResult = CryptDecodeObject(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                            PKCS7_SIGNER_INFO,
                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                            pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                            0,
                            (PVOID)*pCounterSignerInfo,
                            &dwSize);
                        if (!fResult)
                        {
                            __leave;
                        }

                        fReturn = TRUE;

                        break; // Break from for loop.
                    }
                }
            }
            __finally
            {
                // Clean up.
                if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
            }

            return fReturn;
        }

        static BOOL _GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME *st)
        {   
            BOOL fResult;
            FILETIME lft, ft;
            DWORD dwData;
            BOOL fReturn = FALSE;

            // Loop through authenticated attributes and find
            // szOID_RSA_signingTime OID.
            for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
            {           
                if (lstrcmpA(szOID_RSA_signingTime, 
                    pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
                {               
                    // Decode and get FILETIME structure.
                    dwData = sizeof(ft);
                    fResult = CryptDecodeObject(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                        szOID_RSA_signingTime,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                        0,
                        (PVOID)&ft,
                        &dwData);
                    if (!fResult)
                    {
                        break;
                    }

                    // Convert to local time.
                    FileTimeToLocalFileTime(&ft, &lft);
                    FileTimeToSystemTime(&lft, st);

                    fReturn = TRUE;

                    break; // Break from for loop.

                } //lstrcmp szOID_RSA_signingTime
            } // for 

            return fReturn;
        }

    };

}
}
