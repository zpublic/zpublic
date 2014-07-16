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
 * @brief 数字签名相关
 */


#pragma once
#pragma comment(lib, "psapi.lib")

#include "win_utils_header.h"
#include <psapi.h>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <Wintrust.h>
#include <softpub.h>
#include <strsafe.h>

#define WINTRUST_MODULE     "wintrust.dll"
#define CRYPT32_MODULE      "Crypt32.dll"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 数字签名验证相关操作
     */
    class ZLSignVerify {

#include <pshpack8.h>
        typedef struct CATALOG_INFO_
        {
            DWORD   cbStruct;   // set to sizeof(CATALOG_INFO)
            WCHAR   wszCatalogFile[MAX_PATH];
        } CATALOG_INFO;
#include <poppack.h>

        typedef HANDLE          HCATADMIN;
        typedef HANDLE          HCATINFO;
        enum
        {
            MAX_BUFFER_SIZE     = 1024,
            MAX_EXPLORER_COUNT  = 16
        };

        typedef BOOL (WINAPI *CRYPTCATADMINACQUIRECONTEXT)(
            OUT HCATADMIN *phCatAdmin,
            IN const GUID *pgSubsystem,
            IN DWORD dwFlags
            );

        typedef BOOL (WINAPI *CRYPTCATADMINRELEASECONTEXT)(
            IN HCATADMIN hCatAdmin,
            IN DWORD dwFlags
            );

        typedef BOOL (WINAPI *CRYPTCATADMINRELEASECATALOGCONTEXT)(
            IN HCATADMIN hCatAdmin,
            IN HCATINFO hCatInfo,
            IN DWORD dwFlags
            );

        typedef BOOL (WINAPI *CRYPTCATCATALOGINFOFROMCONTEXT)(
            IN HCATINFO hCatInfo,
            IN OUT CATALOG_INFO *psCatInfo,
            IN DWORD dwFlags
            );

        typedef BOOL (WINAPI *CRYPTCATADMINCALCHASHFROMFILEHANDLE)(
            IN HANDLE hFile,
            IN OUT DWORD *pcbHash,
            OUT OPTIONAL BYTE *pbHash,
            IN DWORD dwFlags
            );

        typedef HCATINFO (WINAPI *CRYPTCATADMINENUMCATALOGFROMHASH)(
            IN HCATADMIN hCatAdmin,
            IN BYTE *pbHash,
            IN DWORD cbHash,
            IN DWORD dwFlags,
            IN OUT HCATINFO *phPrevCatInfo
            );

        typedef LONG (WINAPI *WINVERIFYTRUST)(
            HWND hwnd,
            GUID *pgActionID,
            LPVOID pWVTData
            );

        typedef CRYPT_PROVIDER_DATA* (WINAPI *WTHELPERPROVDATAFROMSTATEDATA)(
            HANDLE hStateData
            );

        typedef DWORD (WINAPI *CERTNAMETOSTRW)( 
            IN DWORD dwCertEncodingType, 
            IN PCERT_NAME_BLOB pName, 
            IN DWORD dwStrType, 
            OUT OPTIONAL LPWSTR psz, 
            IN DWORD csz 
            );

        typedef CRYPT_PROVIDER_CERT* (WINAPI *WTHELPERGETPROVCERTFROMCHAIN)(
            CRYPT_PROVIDER_SGNR *pSgnr, 
            DWORD idxCert
            );

        typedef CRYPT_PROVIDER_SGNR* (WINAPI *WTHELPERGETPROVSIGNERFROMCHAIN)(
            CRYPT_PROVIDER_DATA *pProvData, 
            DWORD idxSigner, 
            BOOL fCounterSigner, 
            DWORD idxCounterSigner
            );

    public:
        static ZLSignVerify& Instance() {

            static ZLSignVerify _singleton;
            return _singleton;
        }

        BOOL Initialize() {

            BOOL retval = FALSE;

            if (NULL == m_hDll[0]) {

                m_hDll[0] = ::LoadLibraryA( WINTRUST_MODULE );

                if (NULL == m_hDll[0])
                    goto clean0;
            }

            if (NULL == m_hDll[1]) {

                m_hDll[1] = ::LoadLibraryA( CRYPT32_MODULE );

                if (NULL == m_hDll[1])
                    goto clean0;
            }

            if (NULL == m_pCryptCATAdminAcquireContext) {

                m_pCryptCATAdminAcquireContext = (CRYPTCATADMINACQUIRECONTEXT)::GetProcAddress(
                    m_hDll[0], "CryptCATAdminAcquireContext" );

                if (NULL == m_pCryptCATAdminAcquireContext)
                    goto clean0;
            }

            if (NULL == m_pCryptCATAdminReleaseContext) {

                m_pCryptCATAdminReleaseContext = (CRYPTCATADMINRELEASECONTEXT)::GetProcAddress(
                    m_hDll[0], "CryptCATAdminReleaseContext" );

                if (NULL == m_pCryptCATAdminReleaseContext)
                    goto clean0;
            }

            if (NULL == m_pCryptCATCatalogInfoFromContext) {

                m_pCryptCATCatalogInfoFromContext = (CRYPTCATCATALOGINFOFROMCONTEXT)::GetProcAddress(
                    m_hDll[0], "CryptCATCatalogInfoFromContext" );

                if (NULL == m_pCryptCATCatalogInfoFromContext)
                    goto clean0;
            }

            if (NULL == m_pCryptCATAdminReleaseCatalogContext) {

                m_pCryptCATAdminReleaseCatalogContext = (CRYPTCATADMINRELEASECATALOGCONTEXT)::GetProcAddress(
                    m_hDll[0], "CryptCATAdminReleaseCatalogContext" );

                if (NULL == m_pCryptCATAdminReleaseCatalogContext)
                    goto clean0;
            }

            if (NULL == m_pCryptCATAdminCalcHashFromFileHandle) {

                m_pCryptCATAdminCalcHashFromFileHandle = (CRYPTCATADMINCALCHASHFROMFILEHANDLE)::GetProcAddress(
                    m_hDll[0], "CryptCATAdminCalcHashFromFileHandle" );

                if (NULL == m_pCryptCATAdminCalcHashFromFileHandle)
                    goto clean0;
            }

            if (NULL == m_pCryptCATAdminEnumCatalogFromHash) {

                m_pCryptCATAdminEnumCatalogFromHash = (CRYPTCATADMINENUMCATALOGFROMHASH)::GetProcAddress(
                    m_hDll[0], "CryptCATAdminEnumCatalogFromHash" );

                if (NULL == m_pCryptCATAdminEnumCatalogFromHash)
                    goto clean0;
            }

            if (NULL == m_pWinVerifyTrust) {

                m_pWinVerifyTrust = (WINVERIFYTRUST)::GetProcAddress(
                    m_hDll[0], "WinVerifyTrustEx" );

                if (NULL == m_pWinVerifyTrust)
                    goto clean0;
            }

            if (NULL == m_pWTHelperProvDataFromStateData) {

                m_pWTHelperProvDataFromStateData = (WTHELPERPROVDATAFROMSTATEDATA)::GetProcAddress(
                    m_hDll[0], "WTHelperProvDataFromStateData" );

                if (NULL == m_pWTHelperProvDataFromStateData)
                    goto clean0;
            }

            if (NULL == m_pWTHelperGetProvSignerFromChain) {

                m_pWTHelperGetProvSignerFromChain = (WTHELPERGETPROVSIGNERFROMCHAIN)::GetProcAddress(
                    m_hDll[0], "WTHelperGetProvSignerFromChain" );

                if (NULL == m_pWTHelperGetProvSignerFromChain)
                    goto clean0;
            }

            if (NULL == m_pWTHelperGetProvCertFromChain) {

                m_pWTHelperGetProvCertFromChain = (WTHELPERGETPROVCERTFROMCHAIN)::GetProcAddress(
                    m_hDll[0], "WTHelperGetProvCertFromChain" );

                if (NULL == m_pWTHelperGetProvCertFromChain)
                    goto clean0;
            }

            if (NULL == m_pCertNameToStrW) {

                m_pCertNameToStrW = (CERTNAMETOSTRW)::GetProcAddress(
                    m_hDll[1], "CertNameToStrW" );

                if (NULL == m_pCertNameToStrW)
                    goto clean0;
            }

            RtlZeroMemory(gExploerToken, sizeof(gExploerToken));
            RtlZeroMemory(gNtExplorerName,sizeof(gNtExplorerName));

            QueryExplorerToken();

            retval = TRUE;

clean0:
            if (FALSE == retval) {

                m_pCryptCATAdminAcquireContext = NULL;
                m_pCryptCATAdminReleaseContext = NULL;
                m_pCryptCATAdminReleaseCatalogContext = NULL;
                m_pCryptCATCatalogInfoFromContext = NULL;
                m_pCryptCATAdminCalcHashFromFileHandle = NULL;
                m_pCryptCATAdminEnumCatalogFromHash = NULL;
                m_pWinVerifyTrust = NULL;
                m_pWTHelperProvDataFromStateData = NULL;
                m_pWTHelperGetProvSignerFromChain = NULL;
                m_pWTHelperGetProvCertFromChain = NULL;
                m_pCertNameToStrW = NULL;

                if (m_hDll[0]) {

                    ::FreeLibrary( m_hDll[0] );
                    m_hDll[0] = NULL;
                }

                if (m_hDll[1]) {

                    ::FreeLibrary( m_hDll[1] );
                    m_hDll[1] = NULL;
                }
            }

            return retval;
        }

        VOID UnInitialize() {

            m_pCryptCATAdminAcquireContext = NULL;
            m_pCryptCATAdminReleaseContext = NULL;
            m_pCryptCATAdminReleaseCatalogContext = NULL;
            m_pCryptCATCatalogInfoFromContext = NULL;
            m_pCryptCATAdminCalcHashFromFileHandle = NULL;
            m_pCryptCATAdminEnumCatalogFromHash = NULL;
            m_pWinVerifyTrust = NULL;
            m_pWTHelperProvDataFromStateData = NULL;
            m_pWTHelperGetProvSignerFromChain = NULL;
            m_pWTHelperGetProvCertFromChain = NULL;
            m_pCertNameToStrW = NULL;


            if (m_hDll[0]) {

                ::FreeLibrary( m_hDll[0] );
                m_hDll[0] = NULL;
            }

            if (m_hDll[1]) {

                ::FreeLibrary( m_hDll[1] );
                m_hDll[1] = NULL;
            }
        }

        BOOL QueryExplorerToken()
        {
            DWORD	aProcesses[MAX_BUFFER_SIZE];
            DWORD	cbNeeded;
            DWORD	cProcesses;
            ULONG	i;
            HANDLE	hProcess;
            WCHAR	ProcessFullName[MAX_PATH];
            HANDLE	hToken; 
            BOOL	bFind = FALSE;


            RtlZeroMemory(aProcesses, sizeof(aProcesses));

            if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
                return FALSE;

            cProcesses = cbNeeded / sizeof(DWORD);

            for ( i = 0; i < cProcesses; i++ )
            {
                if(aProcesses[i] == 0 || aProcesses[i] <= 8){
                    continue;
                }

                hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcesses[i]);
                if (!hProcess){
                    continue;
                }

                if (!GetProcessImageFileName(hProcess, ProcessFullName, MAX_PATH)){
                    CloseHandle(hProcess);
                    continue;
                }

                _wcslwr(ProcessFullName);

                if( NULL== wcsstr(ProcessFullName, L"windows\\explorer.exe") ){
                    CloseHandle(hProcess);
                    continue;
                }

                if( !OpenProcessToken(hProcess, TOKEN_ALL_ACCESS_P, &hToken) ){
                    CloseHandle(hProcess);
                    continue;
                }

                for(ULONG j = 0; j < MAX_EXPLORER_COUNT; j++){

                    if (gExploerToken[j] == NULL){
                        gExploerToken[j] = hToken;
                        bFind = TRUE;
                        break;
                    }	
                }

                CloseHandle(hProcess);
            }

            return bFind;
        }

        BOOL VerifyEmbedSignW(LPCWSTR FileFullPath, std::list<std::wstring> &szCertNames, DWORD* pdwRetErr = NULL) {

            BOOL fRetval = FALSE;
            DWORD Err;
            WINTRUST_DATA WintrustData;
            WINTRUST_FILE_INFO WintrustFileInfo;
            GUID DriverVerifyGuid = DRIVER_ACTION_VERIFY;
            GUID WintrustVerifyGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
            CRYPT_PROVIDER_DATA* pDroviderData = NULL;
            CRYPT_PROVIDER_SGNR *pCryptProviderSgnr = NULL;
            CRYPT_PROVIDER_CERT *pCert = NULL;
            HANDLE hFile = INVALID_HANDLE_VALUE;
            BOOL ImpersonateOk = FALSE;


            __try
            {
                if (pdwRetErr != NULL)
                {
                    *pdwRetErr = ERROR_SUCCESS;
                }

                if (NULL == m_pWinVerifyTrust) {
                    return FALSE;
                }

                ZeroMemory( &WintrustData, sizeof(WINTRUST_DATA) );
                WintrustData.cbStruct = sizeof(WINTRUST_DATA);
                WintrustData.dwUIChoice = WTD_UI_NONE;
                WintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
                WintrustData.dwStateAction = WTD_STATEACTION_VERIFY; //WTD_STATEACTION_AUTO_CACHE
                WintrustData.dwProvFlags = WTD_REVOCATION_CHECK_NONE;
                WintrustData.dwUnionChoice = WTD_CHOICE_FILE;
                WintrustData.pFile = &WintrustFileInfo;

                ZeroMemory(&WintrustFileInfo, sizeof(WINTRUST_FILE_INFO));
                WintrustFileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
                WintrustFileInfo.pcwszFilePath = FileFullPath;
                WintrustFileInfo.hFile = NULL;

                hFile = ::CreateFileW(
                    FileFullPath, 
                    GENERIC_READ, 
                    FILE_SHARE_READ | FILE_SHARE_WRITE |FILE_SHARE_DELETE, 
                    NULL, 
                    OPEN_EXISTING, 
                    FILE_ATTRIBUTE_NORMAL, 
                    NULL);

                if (
                    ( INVALID_HANDLE_VALUE == hFile )  &&
                    ( (FileFullPath[0] == L'\\' && FileFullPath[1] == L'\\') || PathIsUNC(FileFullPath) ) 
                    )
                {

                    for(INT i = 0; i < MAX_EXPLORER_COUNT; i++) {

                        ImpersonateOk = ImpersonateLoggedOnUser(gExploerToken[i]);
                        if(ImpersonateOk)
                        {

                            hFile = ::CreateFileW(
                                FileFullPath, 
                                GENERIC_READ,
                                FILE_SHARE_READ | FILE_SHARE_WRITE |FILE_SHARE_DELETE, 
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
                                );

                            if (INVALID_HANDLE_VALUE != hFile)
                            {
                                WintrustFileInfo.pcwszFilePath = NULL;
                                WintrustFileInfo.hFile = hFile;
                                break;
                            }

                            RevertToSelf();
                            ImpersonateOk = FALSE;

                            if (GetLastError() == ERROR_INVALID_HANDLE ||
                                GetLastError() == E_HANDLE)
                            {
                                //无效句柄，重新枚举。
                                QueryExplorerToken();
                                gExploerToken[i] = NULL;
                            }

                        }
                    }

                }else{

                    WintrustFileInfo.pcwszFilePath = NULL;
                    WintrustFileInfo.hFile = hFile;
                }

                //如果打开失败，在走原来方式
                if (INVALID_HANDLE_VALUE == hFile){
                    WintrustFileInfo.pcwszFilePath = FileFullPath;
                    WintrustFileInfo.hFile = NULL;
                }

                Err = (DWORD)m_pWinVerifyTrust( 0,
                    &WintrustVerifyGuid,
                    &WintrustData );

                if (Err == ERROR_SUCCESS) {
                    fRetval = GetCertNames( WintrustData.hWVTStateData, szCertNames );
                }
                else
                {
                    if (pdwRetErr != NULL)
                    {
                        *pdwRetErr = Err;
                    }
                }

                /// add by yangjun mem leak
                if (WintrustData.hWVTStateData)
                {
                    WintrustData.dwStateAction = WTD_STATEACTION_CLOSE; 
                    (DWORD)m_pWinVerifyTrust( 0,
                        &WintrustVerifyGuid,
                        &WintrustData );
                }

                if (hFile != INVALID_HANDLE_VALUE) {
                    ::CloseHandle(hFile);
                    hFile = INVALID_HANDLE_VALUE;
                }

                if(ImpersonateOk)
                {
                    RevertToSelf();
                    ImpersonateOk = FALSE;
                }

            }__except(EXCEPTION_EXECUTE_HANDLER){

                if(ImpersonateOk)
                {
                    RevertToSelf();
                }

                if (hFile != INVALID_HANDLE_VALUE) {
                    ::CloseHandle(hFile);
                }
            }

            return fRetval;
        }

        BOOL IsTrustedStatData( HANDLE hWVTStateData )
        {
            CRYPT_PROVIDER_DATA* pDroviderData = m_pWTHelperProvDataFromStateData( hWVTStateData );

            if (NULL == pDroviderData) {
                return FALSE;
            }

            for (int i = 0; i < (int)pDroviderData->csSigners; ++i) {

                CRYPT_PROVIDER_SGNR *pCryptProviderSgnr = m_pWTHelperGetProvSignerFromChain(
                    pDroviderData, 0, FALSE, 0 );

                if (NULL == pCryptProviderSgnr) {

                    break;
                }

                for (int j = 0; j < (int)pCryptProviderSgnr->csCertChain; j++) {

                    CRYPT_PROVIDER_CERT *pCert = m_pWTHelperGetProvCertFromChain(pCryptProviderSgnr, j);

                    if (NULL == pCert) {

                        break;
                    }

                    WCHAR wszInfo[1024] = {0};
                    DWORD dwRetCode = m_pCertNameToStrW( X509_ASN_ENCODING, 
                        &(pCert->pCert->pCertInfo->Subject),
                        CERT_X500_NAME_STR | CERT_NAME_STR_REVERSE_FLAG,
                        wszInfo,
                        1024 );

                    if (dwRetCode) {
                        if (IsBlackOu( wszInfo ))
                            return TRUE;
                    }
                }

            }
            return FALSE;
        }

        BOOL GetEmbedSignSubject(LPCSTR FileFullPath, std::vector<std::string>& subInfo)
        {
            BOOL retval = FALSE;
            PWCHAR wszFullPath = NULL;
            int nRetCode;
            int nStrLen;

            nStrLen = (int)strlen( FileFullPath );
            wszFullPath = new WCHAR[nStrLen + 1];

            if (NULL == wszFullPath)
                goto clean0;

            nRetCode = MultiByteToWideChar( CP_ACP,
                0,
                FileFullPath,
                -1,
                wszFullPath,
                nStrLen + 1 );

            if (nRetCode) {

                retval = GetEmbedSignSubject( wszFullPath , subInfo);

            } else {

                retval = FALSE;
            }
clean0:
            if (wszFullPath) {

                delete[] wszFullPath;
                wszFullPath = NULL;
            }

            return retval;
        }

        // 获取数字签名链的主题信息
        BOOL GetEmbedSignSubject(LPCWSTR FileFullPath, std::vector<std::string>& subInfo)
        {
            BOOL fRetval = FALSE;
            DWORD Err;
            WINTRUST_DATA WintrustData;
            WINTRUST_FILE_INFO WintrustFileInfo;
            GUID DriverVerifyGuid = DRIVER_ACTION_VERIFY;
            GUID WintrustVerifyGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
            CRYPT_PROVIDER_DATA* pDroviderData = NULL;
            PWCHAR wszInfo = NULL;
            CRYPT_PROVIDER_SGNR *pCryptProviderSgnr = NULL;
            CRYPT_PROVIDER_CERT *pCert = NULL;

            if (NULL == m_pWinVerifyTrust) {

                return FALSE;
            }

            wszInfo = new WCHAR[1024];

            if (NULL == wszInfo)
                goto clean0;

            ZeroMemory( &WintrustData, sizeof(WINTRUST_DATA) );
            WintrustData.cbStruct = sizeof(WINTRUST_DATA);
            WintrustData.dwUIChoice = WTD_UI_NONE;
            WintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
            WintrustData.dwStateAction = WTD_STATEACTION_VERIFY;
            WintrustData.dwProvFlags = WTD_REVOCATION_CHECK_NONE;
            WintrustData.dwUnionChoice = WTD_CHOICE_FILE;
            WintrustData.pFile = &WintrustFileInfo;

            ZeroMemory(&WintrustFileInfo, sizeof(WINTRUST_FILE_INFO));
            WintrustFileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
            WintrustFileInfo.pcwszFilePath = FileFullPath;

            Err = (DWORD)m_pWinVerifyTrust( NULL,
                &WintrustVerifyGuid,
                &WintrustData );

            if (Err != ERROR_SUCCESS) {

                goto clean0;
            }

            fRetval = GetProvDataFromTrustHandle( WintrustData.hWVTStateData, subInfo );

clean0:
            if (wszInfo) {

                delete[] wszInfo;
                wszInfo = NULL;
            }

            return fRetval;
        }

        BOOL GetProvDataFromTrustHandle( HANDLE hWVTStateData, std::vector<std::string>& subInfo )
        {
            BOOL bRet = FALSE;
            CRYPT_PROVIDER_DATA* pDroviderData = m_pWTHelperProvDataFromStateData( hWVTStateData );

            if (NULL == pDroviderData) {
                return bRet;
            }

            for (int i = 0; i < (int)pDroviderData->csSigners; ++i) {

                CRYPT_PROVIDER_SGNR *pCryptProviderSgnr = m_pWTHelperGetProvSignerFromChain(
                    pDroviderData, 0, FALSE, 0 );

                if (NULL == pCryptProviderSgnr) {

                    break;
                }

                for (int j = 0; j < (int)pCryptProviderSgnr->csCertChain; j++) {

                    CRYPT_PROVIDER_CERT *pCert = m_pWTHelperGetProvCertFromChain(pCryptProviderSgnr, j);

                    if (NULL == pCert) {

                        break;
                    }

                    WCHAR wszInfo[1024] = {0};
                    DWORD dwRetCode = m_pCertNameToStrW( X509_ASN_ENCODING, 
                        &(pCert->pCert->pCertInfo->Subject),
                        CERT_X500_NAME_STR | CERT_NAME_STR_REVERSE_FLAG,
                        wszInfo,
                        1024 );

                    if (dwRetCode) 
                    {
                        if (IsTrusted( wszInfo ))
                        {
                            CHAR szInfo[1024] = {0};

                            dwRetCode = WideCharToMultiByte( CP_ACP,
                                0,
                                wszInfo,
                                -1,
                                szInfo,
                                1024,
                                NULL,
                                NULL );

                            if (0 == dwRetCode)
                            {
                                subInfo.push_back(std::string("Error: Subject cannot change to multibyte!"));
                                continue;
                            }
                            else
                            {
                                subInfo.push_back(std::string(szInfo));
                                bRet = TRUE;
                            }
                        }
                    }
                }

            }
            return bRet;
        }

        BOOL VerifyCatSignA(LPCSTR FileFullPath) {

            DWORD retval = FALSE;
            PWCHAR wszFullPath = NULL;
            int nRetCode;
            int nStrLen;

            nStrLen = (int)strlen( FileFullPath );
            wszFullPath = new WCHAR[nStrLen + 1];

            if (NULL == wszFullPath)
                goto clean0;

            nRetCode = MultiByteToWideChar( CP_ACP,
                0,
                FileFullPath,
                -1,
                wszFullPath,
                nStrLen + 1 );

            if (nRetCode) {

                retval = VerifyCatSignW( wszFullPath );

            } else {

                retval = FALSE;
            }

clean0:
            if (wszFullPath) {

                delete[] wszFullPath;
                wszFullPath = NULL;
            }

            return retval;
        }


        BOOL VerifyCatSignW(LPCWSTR FileFullPath) {

            BOOL fRetval = FALSE;
            LPBYTE Hash = NULL;
            DWORD HashSize;
            CATALOG_INFO CatInfo;
            HANDLE hFile = NULL;
            HCATADMIN hCatAdmin;
            HCATINFO hCatInfo;
            HCATINFO PrevCat;
            WINTRUST_DATA WintrustData;
            WINTRUST_CATALOG_INFO WintrustCatalogInfo;
            WCHAR UnicodeKey[MAX_PATH];
            GUID DriverVerifyGuid = DRIVER_ACTION_VERIFY;
            PWCHAR Key = NULL;

            if (NULL == m_pWinVerifyTrust) {

                return FALSE;
            }

            StringCchCopyW( UnicodeKey, MAX_PATH, FileFullPath );
            Key = MyGetFileTitle( UnicodeKey );

            if (!m_pCryptCATAdminAcquireContext( &hCatAdmin, &DriverVerifyGuid, 0 )) {

                goto clean0;
            }

            hFile = CreateFileW( FileFullPath,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE |FILE_SHARE_DELETE,
                NULL,
                OPEN_EXISTING,
                0,
                NULL );

            if (INVALID_HANDLE_VALUE == hFile) {

                goto clean0;
            }

            ZeroMemory(&WintrustData, sizeof(WINTRUST_DATA));
            WintrustData.cbStruct = sizeof(WINTRUST_DATA);
            WintrustData.dwUIChoice = WTD_UI_NONE;
            WintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
            WintrustData.dwStateAction = WTD_STATEACTION_AUTO_CACHE;
            WintrustData.dwProvFlags = WTD_REVOCATION_CHECK_NONE;

            HashSize = 100;
            Hash = new BYTE[HashSize];

            if(NULL == Hash) {

                goto clean0;
            }

            if (!m_pCryptCATAdminCalcHashFromFileHandle( hFile, &HashSize, Hash, 0 )) {

                goto clean0;
            }

            WintrustData.dwUnionChoice = WTD_CHOICE_CATALOG;
            WintrustData.pCatalog = &WintrustCatalogInfo;

            ZeroMemory(&WintrustCatalogInfo, sizeof(WINTRUST_CATALOG_INFO));
            WintrustCatalogInfo.cbStruct = sizeof(WINTRUST_CATALOG_INFO);
            WintrustCatalogInfo.pbCalculatedFileHash = Hash;
            WintrustCatalogInfo.cbCalculatedFileHash = HashSize;

            StringCchCopyW( UnicodeKey, MAX_PATH, Key);
            CharLowerW( UnicodeKey );

            WintrustCatalogInfo.pcwszMemberTag = UnicodeKey;

            PrevCat = NULL;
            hCatInfo = m_pCryptCATAdminEnumCatalogFromHash(
                hCatAdmin,
                Hash,
                HashSize,
                0,
                &PrevCat );

            while (hCatInfo) {
                CatInfo.cbStruct = sizeof(CATALOG_INFO);
                if (m_pCryptCATCatalogInfoFromContext( hCatInfo, &CatInfo, 0 )) {

                    // if (VerifyEmbedSignW( CatInfo.wszCatalogFile ))
                    {
                        // m_pCryptCATAdminReleaseCatalogContext( hCatAdmin, hCatInfo, 0 );
                        fRetval = TRUE;
                        break;
                    }                
                }

                PrevCat = hCatInfo;

                hCatInfo = m_pCryptCATAdminEnumCatalogFromHash(
                    hCatAdmin, Hash, HashSize, 0, &PrevCat );

            }

clean0:
            if (Hash) {

                delete[] Hash;
                Hash = NULL;
            }

            if (hCatAdmin) {

                m_pCryptCATAdminReleaseContext( hCatAdmin, 0 );
            }

            if (hFile) {

                ::CloseHandle( hFile );
                hFile = NULL;
            }

            return fRetval;
        }

        BOOL GetCatSignSubject(LPCSTR FileFullPath, std::vector<std::string>& subInfo)
        {
            BOOL retval = FALSE;
            PWCHAR wszFullPath = NULL;
            int nRetCode;
            int nStrLen;

            nStrLen = (int)strlen( FileFullPath );
            wszFullPath = new WCHAR[nStrLen + 1];

            if (NULL == wszFullPath)
                goto clean0;

            nRetCode = MultiByteToWideChar( CP_ACP,
                0,
                FileFullPath,
                -1,
                wszFullPath,
                nStrLen + 1 );

            if (nRetCode) {

                retval = GetCatSignSubject( wszFullPath , subInfo);

            } else {

                retval = FALSE;
            }
clean0:
            if (wszFullPath) {

                delete[] wszFullPath;
                wszFullPath = NULL;
            }

            return retval;
        }

        BOOL GetCatSignSubject(LPCWSTR FileFullPath, std::vector<std::string>& subInfo)
        {
            BOOL fRetval = FALSE;
            LPBYTE Hash = NULL;
            DWORD HashSize;
            CATALOG_INFO CatInfo;
            HANDLE hFile = NULL;
            HCATADMIN hCatAdmin;
            HCATINFO hCatInfo;
            HCATINFO PrevCat;
            DWORD Err;
            WINTRUST_DATA WintrustData;
            WINTRUST_CATALOG_INFO WintrustCatalogInfo;
            LPWSTR CatalogFullPath;
            WCHAR UnicodeKey[MAX_PATH];
            GUID DriverVerifyGuid = DRIVER_ACTION_VERIFY;
            PWCHAR Key = NULL;

            if (NULL == m_pWinVerifyTrust) {

                return FALSE;
            }

            StringCchCopyW( UnicodeKey, MAX_PATH, FileFullPath );
            Key = MyGetFileTitle( UnicodeKey );

            if (!m_pCryptCATAdminAcquireContext( &hCatAdmin, &DriverVerifyGuid, 0 )) {

                goto clean0;
            }

            hFile = CreateFileW( FileFullPath,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE |FILE_SHARE_DELETE,
                NULL,
                OPEN_EXISTING,
                0,
                NULL );

            if (INVALID_HANDLE_VALUE == hFile) {

                goto clean0;
            }

            ZeroMemory(&WintrustData, sizeof(WINTRUST_DATA));
            WintrustData.cbStruct = sizeof(WINTRUST_DATA);
            WintrustData.dwUIChoice = WTD_UI_NONE;
            WintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
            WintrustData.dwStateAction = WTD_STATEACTION_AUTO_CACHE;
            WintrustData.dwProvFlags = WTD_REVOCATION_CHECK_NONE;

            HashSize = 100;
            Hash = new BYTE[HashSize];

            if(NULL == Hash) {

                goto clean0;
            }

            if (!m_pCryptCATAdminCalcHashFromFileHandle( hFile, &HashSize, Hash, 0 )) {

                goto clean0;
            }

            WintrustData.dwUnionChoice = WTD_CHOICE_CATALOG;
            WintrustData.pCatalog = &WintrustCatalogInfo;

            ZeroMemory(&WintrustCatalogInfo, sizeof(WINTRUST_CATALOG_INFO));
            WintrustCatalogInfo.cbStruct = sizeof(WINTRUST_CATALOG_INFO);
            WintrustCatalogInfo.pbCalculatedFileHash = Hash;
            WintrustCatalogInfo.cbCalculatedFileHash = HashSize;

            StringCchCopyW( UnicodeKey, MAX_PATH, Key);
            CharLowerW( UnicodeKey );

            WintrustCatalogInfo.pcwszMemberTag = UnicodeKey;

            PrevCat = NULL;
            hCatInfo = m_pCryptCATAdminEnumCatalogFromHash(
                hCatAdmin,
                Hash,
                HashSize,
                0,
                &PrevCat );

            while (hCatInfo) {
                CatInfo.cbStruct = sizeof(CATALOG_INFO);
                if (m_pCryptCATCatalogInfoFromContext( hCatInfo, &CatInfo, 0 )) {

                    CatalogFullPath = CatInfo.wszCatalogFile;
                    WintrustCatalogInfo.pcwszCatalogFilePath = CatInfo.wszCatalogFile;

                    Err = (DWORD)m_pWinVerifyTrust(NULL,
                        &DriverVerifyGuid,
                        &WintrustData
                        );

                    if (NO_ERROR  == Err) {

                        if (GetEmbedSignSubject(CatInfo.wszCatalogFile, subInfo))
                        {
                            CHAR szInfo[1024] = {0};

                            WideCharToMultiByte( CP_ACP,
                                0,
                                CatInfo.wszCatalogFile,
                                -1,
                                szInfo,
                                1024,
                                NULL,
                                NULL );
                            subInfo.push_back(std::string(szInfo));
                            // m_pCryptCATAdminReleaseCatalogContext( hCatAdmin, hCatInfo, 0 );

                            fRetval = TRUE;
                            break;
                        }
                    }
                }

                PrevCat = hCatInfo;

                hCatInfo = m_pCryptCATAdminEnumCatalogFromHash(
                    hCatAdmin, Hash, HashSize, 0, &PrevCat );

            }

clean0:
            if (Hash) {

                delete[] Hash;
                Hash = NULL;
            }

            if (hCatAdmin) {

                m_pCryptCATAdminReleaseContext( hCatAdmin, 0 );
            }

            if (hFile) {

                ::CloseHandle( hFile );
                hFile = NULL;
            }

            return fRetval;
        }

        void AddBlackName(LPCWSTR lpName)
        {
            if( lpName != NULL )
                m_blackNames.insert( lpName );
        }

        void AddTrustModule(LPCWSTR lpName)
        {
            USES_CONVERSION;
            AddTrustModule(W2A(lpName));
        }

        void AddTrustModule(LPCSTR lpName) {

            std::vector<std::string>::const_iterator it;
            BOOL fExist = FALSE;

            size_t len = strlen(lpName);
            LPSTR str = new char[len + 1];
            strcpy_s(str, len + 1, lpName);
            _strlwr(str);

            for (it = m_trustNames.begin(); it != m_trustNames.end(); ++it) {

                if (_stricmp( str, it->c_str() ) == 0) {

                    fExist = TRUE;
                    break;
                }
            }

            if (FALSE == fExist) {

                m_trustNames.push_back( str );
            }
            delete [] str;
        }

    private:
        ZLSignVerify()
            : m_pCryptCATAdminAcquireContext( NULL )
            , m_pCryptCATAdminReleaseContext( NULL )
            , m_pCryptCATAdminReleaseCatalogContext( NULL )
            , m_pCryptCATCatalogInfoFromContext( NULL )
            , m_pCryptCATAdminCalcHashFromFileHandle( NULL )
            , m_pCryptCATAdminEnumCatalogFromHash( NULL )
            , m_pWinVerifyTrust( NULL )
            , m_pWTHelperProvDataFromStateData( NULL )
            , m_pWTHelperGetProvSignerFromChain( NULL )
            , m_pWTHelperGetProvCertFromChain( NULL )
            , m_pCertNameToStrW( NULL )
        {
            m_hDll[0] = m_hDll[1] = NULL;
        }

        ~ZLSignVerify() {

        }

        PWSTR MyGetFileTitle(PWSTR FilePath) {

            PWSTR LastComponent = FilePath;
            WCHAR  CurChar;

            while (CurChar = *FilePath) {

                FilePath = CharNextW( FilePath );
                if ((CurChar == TEXT('\\')) || (CurChar == TEXT('/')) || (CurChar == TEXT(':'))) {

                    LastComponent = FilePath;
                }
            }

            return LastComponent;
        }

        BOOL CompareStr(LPCSTR s1, LPCSTR s2)
        {
            while ( (*s1) && (*s2) )
            {
                if (*s1 != *s2)
                    return FALSE;
                s1++;
                s2++;
            }
            return TRUE;
        }

        BOOL GetCertNames(HANDLE hWVTStateData, std::list<std::wstring> &szCertNames)
        {
            CRYPT_PROVIDER_DATA* pDroviderData = m_pWTHelperProvDataFromStateData( hWVTStateData );

            if (NULL == pDroviderData) {
                return FALSE;
            }

            for (int i = 0; i < (int)pDroviderData->csSigners; ++i) {

                CRYPT_PROVIDER_SGNR *pCryptProviderSgnr = m_pWTHelperGetProvSignerFromChain(
                    pDroviderData, 0, FALSE, 0 );

                if (NULL == pCryptProviderSgnr) {

                    break;
                }

                for (int j = 0; j < (int)pCryptProviderSgnr->csCertChain; j++) {

                    CRYPT_PROVIDER_CERT *pCert = m_pWTHelperGetProvCertFromChain(pCryptProviderSgnr, j);

                    if (NULL == pCert) {

                        break;
                    }

                    WCHAR wszInfo[1024] = {0};
                    DWORD dwRetCode = m_pCertNameToStrW( X509_ASN_ENCODING, 
                        &(pCert->pCert->pCertInfo->Subject),
                        CERT_X500_NAME_STR | CERT_NAME_STR_REVERSE_FLAG,
                        wszInfo,
                        1024 );

                    if (dwRetCode) {
                        szCertNames.push_back( wszInfo );
                    }
                }

            }

            return !szCertNames.empty();
        }

        BOOL IsBlackOu(PWCHAR wszInfo)
        {
            if( wszInfo == NULL )
                return FALSE;

            _wcslwr_s(wszInfo, 1024);

            for( std::set<std::wstring>::iterator item = m_blackNames.begin(); item != m_blackNames.end(); item++ )
            {
                if( item->find(wszInfo) != std::string::npos )
                {
                    return TRUE;
                }
            }
            return FALSE;
        }

        BOOL IsTrusted(PWCHAR wszInfo) {

            BOOL fRetval = FALSE;
            PCHAR szInfo = NULL;
            int nRetCode;
            int i;

            int nlen = (int)wcslen(wszInfo);

            szInfo = new CHAR[nlen + 2];
            memset(szInfo, 0,nlen + 2);

            if (NULL == szInfo)
                goto clean0;

            nRetCode = WideCharToMultiByte( CP_ACP,
                0,
                wszInfo,
                -1,
                szInfo,
                nlen+1,
                NULL,
                NULL );
            if (0 == nRetCode)
                goto clean0;

            _strlwr(szInfo);

            for (i = 0; i < (int)m_trustNames.size(); ++i) {
                const char* p = m_trustNames[i].c_str();
                if ( (_stricmp( szInfo, p ) == 0) ||
                    CompareStr( szInfo, p) )
                {
                    fRetval = TRUE;
                    goto clean0;
                }
            }

clean0:
            if (szInfo) {

                delete[] szInfo;
                szInfo = NULL;
            }

            return fRetval;
        }

        HMODULE m_hDll[2];

        CRYPTCATADMINACQUIRECONTEXT m_pCryptCATAdminAcquireContext;
        CRYPTCATADMINRELEASECONTEXT m_pCryptCATAdminReleaseContext;
        CRYPTCATADMINRELEASECATALOGCONTEXT m_pCryptCATAdminReleaseCatalogContext;
        CRYPTCATCATALOGINFOFROMCONTEXT m_pCryptCATCatalogInfoFromContext;
        CRYPTCATADMINCALCHASHFROMFILEHANDLE m_pCryptCATAdminCalcHashFromFileHandle;
        CRYPTCATADMINENUMCATALOGFROMHASH m_pCryptCATAdminEnumCatalogFromHash;
        WINVERIFYTRUST m_pWinVerifyTrust;
        WTHELPERPROVDATAFROMSTATEDATA m_pWTHelperProvDataFromStateData;
        WTHELPERGETPROVSIGNERFROMCHAIN m_pWTHelperGetProvSignerFromChain;
        WTHELPERGETPROVCERTFROMCHAIN m_pWTHelperGetProvCertFromChain;
        CERTNAMETOSTRW m_pCertNameToStrW;

        std::vector<std::string> m_trustNames;
        std::set<std::wstring> m_blackNames;

        HANDLE	gExploerToken[MAX_EXPLORER_COUNT];
        WCHAR	gNtExplorerName[MAX_PATH/2];
    };

}
}
