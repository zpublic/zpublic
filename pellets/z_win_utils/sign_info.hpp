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
 * @brief 数字签名信息获取
 * @see http://support.microsoft.com/kb/323809/zh-cn
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
 * @brief 数字签名信息读取接口
 */
class ZLSignInfo
{
public:
    BOOL Load(LPCTSTR lpFilePath);

    ///> 签名者
    CString GetNameOfSigner() const;
    ///> 颁发者
    CString GetNameOfIssuer() const;
    ///> 签名时间
    SYSTEMTIME GetSigningTime() const;
    ///> 证书序列号
    CString GetSerialNumber() const;

    ///> 判断签名是否有时间戳(即时间戳在指定年限内)
    static BOOL IsDigitalSignatureHasTimestamp( LPCWSTR lpFilePath, WORD wMinYear = 2000, WORD wMaxYear = 2100);

private:
    void _Clear();
    static void              _GetFileSignerHandle(LPCTSTR lpFilePath, HCRYPTMSG& hCryptMsg, HCERTSTORE& hCertStore);
    static PCMSG_SIGNER_INFO _GetMsgSignerInfoOfFile(HCRYPTMSG hCryptMsg);
    static PCMSG_SIGNER_INFO _GetMsgSignerInfoOfTimestamp(PCMSG_SIGNER_INFO pMsgSignerInfoOfFile);
    static SYSTEMTIME        _ReadSigningTime(PCMSG_SIGNER_INFO pFileSignerInfo);
    static CString           _ReadSerialNumber(PCCERT_CONTEXT pCertContext);
    static CString           _ReadSignerName(PCCERT_CONTEXT pCertContext);
    static CString           _ReadIssuerName(PCCERT_CONTEXT pCertContext);
    static CString           _SimpleCertContextReader(PCCERT_CONTEXT pCertContext, DWORD dwType, DWORD dwFlags);
    static void              _ReleaseMsgSignerInfoPtr(PCMSG_SIGNER_INFO p);
    static PCCERT_CONTEXT    _ReadCertInfo(HCERTSTORE hCertStore, const PCMSG_SIGNER_INFO pMsgSignerInfoOfFile);

private:
    CString     m_sSignerName;    // 使用者
    CString     m_sIssuerName;    // 颁发者
    SYSTEMTIME  m_tSigningTime;   // 签名时间
    CString     m_sSerialNumber;  // 证书序列号
};

// 以下是实现部分

inline CString    ZLSignInfo::GetNameOfSigner() const { return m_sSignerName;   }
inline CString    ZLSignInfo::GetNameOfIssuer() const { return m_sIssuerName;   }
inline SYSTEMTIME ZLSignInfo::GetSigningTime()  const { return m_tSigningTime;  }
inline CString    ZLSignInfo::GetSerialNumber() const { return m_sSerialNumber; }

inline BOOL ZLSignInfo::Load( LPCTSTR lpFilePath )
{
    _Clear();

    HCRYPTMSG         hCryptMsg = NULL;
    HCERTSTORE        hCertStore = NULL;
    PCCERT_CONTEXT    pCertContext = NULL;
    PCMSG_SIGNER_INFO pMsgSignerInfoOfFile = NULL;
    PCMSG_SIGNER_INFO pMsgSignerInfoOfTimestamp = NULL;

    _GetFileSignerHandle(lpFilePath, hCryptMsg, hCertStore);

    if (NULL == hCryptMsg || NULL == hCertStore)
        return FALSE;

    if (hCryptMsg)
    {
        pMsgSignerInfoOfFile = _GetMsgSignerInfoOfFile(hCryptMsg);
    }
    if (pMsgSignerInfoOfFile)
    {
        pMsgSignerInfoOfTimestamp = _GetMsgSignerInfoOfTimestamp(pMsgSignerInfoOfFile);
    }
    if (pMsgSignerInfoOfTimestamp)
    {
        m_tSigningTime = _ReadSigningTime(pMsgSignerInfoOfTimestamp);
    }
    pCertContext    = _ReadCertInfo(hCertStore, pMsgSignerInfoOfFile);
    m_sSignerName   = _ReadSignerName(pCertContext);
    m_sIssuerName   = _ReadIssuerName(pCertContext);
    m_sSerialNumber = _ReadSerialNumber(pCertContext);

    if (hCryptMsg)                 ::CryptMsgClose(hCryptMsg);
    if (hCertStore)                ::CertCloseStore(hCertStore, 0);
    if (pCertContext)              ::CertFreeCertificateContext(pCertContext);
    if (pMsgSignerInfoOfFile)      ::LocalFree((HLOCAL)pMsgSignerInfoOfFile);
    if (pMsgSignerInfoOfTimestamp) ::LocalFree((HLOCAL)pMsgSignerInfoOfTimestamp);

    return TRUE;
}

inline BOOL ZLSignInfo::IsDigitalSignatureHasTimestamp( LPCWSTR lpFilePath, WORD wMinYear, WORD wMaxYear)
{
    ZLSignInfo signer;
    if (signer.Load(lpFilePath))
    {
        SYSTEMTIME st = signer.GetSigningTime();
        if (st.wYear >= wMinYear && st.wYear <=wMaxYear)
        {
            return TRUE;
        }
    }
    return FALSE;
}

inline void ZLSignInfo::_Clear()
{
    m_sSignerName.Empty();
    m_sIssuerName.Empty();
    m_sSerialNumber.Empty();
    memset(&m_tSigningTime, 0, sizeof(SYSTEMTIME));
}

inline SYSTEMTIME ZLSignInfo::_ReadSigningTime(PCMSG_SIGNER_INFO pMsgSignerInfoOfTimestamp)
{
    SYSTEMTIME st = {0};

    for (DWORD n = 0; n < pMsgSignerInfoOfTimestamp->AuthAttrs.cAttr; ++n)
    {
        if (lstrcmpA(szOID_RSA_signingTime, pMsgSignerInfoOfTimestamp->AuthAttrs.rgAttr[n].pszObjId) == 0)
        {
            FILETIME ftSignInfo = {0};
            DWORD dwSize = sizeof(FILETIME);

            if (::CryptDecodeObject(
                X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                szOID_RSA_signingTime,
                pMsgSignerInfoOfTimestamp->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                pMsgSignerInfoOfTimestamp->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                0,
                (PVOID)&ftSignInfo,
                &dwSize))
            {
                FILETIME ftTmp = {0};
                ::FileTimeToLocalFileTime(&ftSignInfo, &ftTmp);
                ::FileTimeToSystemTime(&ftTmp, &st);
            }

            break;
        }
    }
    return st;
}

inline PCMSG_SIGNER_INFO ZLSignInfo::_GetMsgSignerInfoOfFile(HCRYPTMSG hCryptMsg) 
{
    PCMSG_SIGNER_INFO pMsgSignerInfoOfFile = NULL;
    if (hCryptMsg)
    {
        BOOL bRet = FALSE;
        // 取大小
        DWORD dwMsgSignerInfoSize = 0;
        bRet = ::CryptMsgGetParam(hCryptMsg, CMSG_SIGNER_INFO_PARAM, 0, NULL, &dwMsgSignerInfoSize);
        if (!bRet || 0==dwMsgSignerInfoSize)
        {
            goto Exit0;
        }

        // 申请内存
        pMsgSignerInfoOfFile = (PCMSG_SIGNER_INFO)::LocalAlloc(LPTR, dwMsgSignerInfoSize);
        if (NULL == pMsgSignerInfoOfFile)
        {
            goto Exit0;
        }

        // 取值
        bRet = ::CryptMsgGetParam(hCryptMsg, CMSG_SIGNER_INFO_PARAM, 0, (void*)pMsgSignerInfoOfFile, &dwMsgSignerInfoSize);
        if (!bRet)
        {
            goto Exit0;
        }
    }
Exit0:
    return pMsgSignerInfoOfFile;
}

inline PCMSG_SIGNER_INFO ZLSignInfo::_GetMsgSignerInfoOfTimestamp( PCMSG_SIGNER_INFO pMsgSignerInfoOfFile )
{
    PCMSG_SIGNER_INFO pMsgSignerInfoOfTimestamp = NULL;

    for (DWORD n=0; n<pMsgSignerInfoOfFile->UnauthAttrs.cAttr; n++)
    {
        if (lstrcmpA(pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].pszObjId, szOID_RSA_counterSign) == 0)
        {
            BOOL bRet = FALSE;

            // 取大小
            DWORD dwSize = 0;
            bRet = ::CryptDecodeObject(
                X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                PKCS7_SIGNER_INFO,
                pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                0,
                NULL,
                &dwSize);
            if (!bRet || dwSize<=0)
            {
                goto Exit0;
            }

            // 申请内存
            pMsgSignerInfoOfTimestamp = (PCMSG_SIGNER_INFO)::LocalAlloc(LPTR, dwSize);
            if (NULL == pMsgSignerInfoOfTimestamp)
            {
                goto Exit0;
            }

            // 取值
            bRet = ::CryptDecodeObject(
                X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                PKCS7_SIGNER_INFO,
                pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                pMsgSignerInfoOfFile->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                0,
                pMsgSignerInfoOfTimestamp,
                &dwSize);
            if (!bRet)
            {
                goto Exit0;
            }
        }
    }

Exit0:
    return pMsgSignerInfoOfTimestamp;
}

inline PCCERT_CONTEXT ZLSignInfo::_ReadCertInfo( HCERTSTORE hCertStore, const PCMSG_SIGNER_INFO pMsgSignerInfoOfFile )
{
    PCCERT_CONTEXT pCertContext = NULL;

    if (hCertStore && pMsgSignerInfoOfFile)
    {
        CERT_INFO stCertInfo;
        stCertInfo.Issuer       = pMsgSignerInfoOfFile->Issuer;
        stCertInfo.SerialNumber = pMsgSignerInfoOfFile->SerialNumber;

        pCertContext = ::CertFindCertificateInStore(hCertStore,
            X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
            0,
            CERT_FIND_SUBJECT_CERT,
            (PVOID)&stCertInfo,
            NULL);
    }

    return pCertContext;
}

inline void ZLSignInfo::_GetFileSignerHandle( LPCTSTR lpFilePath, HCRYPTMSG& hCryptMsg, HCERTSTORE& hCertStore )
{
    hCryptMsg  = NULL;
    hCertStore = NULL;
    if (::PathFileExists(lpFilePath))
    {
        if (FALSE == ::CryptQueryObject(
            CERT_QUERY_OBJECT_FILE,
            lpFilePath,
            CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
            CERT_QUERY_FORMAT_FLAG_BINARY,
            0,
            NULL,
            NULL,
            NULL,
            &hCertStore,
            &hCryptMsg,
            NULL))
        {
            hCryptMsg  = NULL;
            hCertStore = NULL;
        }
    }
}

inline CString ZLSignInfo::_ReadSerialNumber(PCCERT_CONTEXT pCertContext)
{
    CString sSerialNumber;
    if (pCertContext)
    {
        DWORD dwSize = pCertContext->pCertInfo->SerialNumber.cbData;
        for (DWORD n=0; n<dwSize; n++)
        {
            sSerialNumber.AppendFormat(_T("%02x"), pCertContext->pCertInfo->SerialNumber.pbData[dwSize - (n + 1)]);
        }
    }
    return sSerialNumber;
}

inline CString ZLSignInfo::_ReadSignerName(PCCERT_CONTEXT pCertContext)
{
    return _SimpleCertContextReader(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0);
}

inline CString ZLSignInfo::_ReadIssuerName(PCCERT_CONTEXT pCertContext)
{
    return _SimpleCertContextReader(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG);
}

inline CString ZLSignInfo::_SimpleCertContextReader(PCCERT_CONTEXT pCertContext, DWORD dwType, DWORD dwFlags)
{
    CString sReturn;
    TCHAR*  pBuf = NULL;
    if (pCertContext)
    {
        // 取大小
        DWORD dwSize = ::CertGetNameString(pCertContext, dwType, dwFlags, NULL, NULL, 0);
        if (dwSize <= 0)
        {
            goto Exit0;
        }
        // 申请内存
        pBuf = (LPTSTR)::LocalAlloc(LPTR, dwSize * sizeof(TCHAR));
        if (NULL == pBuf)
        {
            goto Exit0;
        }
        // 取值
        ::CertGetNameString(pCertContext, dwType, dwFlags, NULL, pBuf, dwSize);
        if (NULL == pBuf)
        {
            goto Exit0;
        }

        sReturn = pBuf;
    }

Exit0:
    if (pBuf)
    {
        ::LocalFree((HLOCAL)pBuf);
    }

    return sReturn;
}

} // end of namespace WinUtils
} // end of namespace zl
