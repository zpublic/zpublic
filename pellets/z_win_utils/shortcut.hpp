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
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{

    class ZLShortcut
    {

        enum
        {
            WIN_PATH_MAX_UNICODE_PATH           = 32767 + 4,    ///> max size of buffer to store path, in bytes
            WIN_PATH_UNICODE_PATH_PREFIX        = 4,            ///> length of "\\\\??\\"
            WIN_PATH_UNICODE_UNC_PATH_PREFIX    = 7,            ///> length of "\\\\??\\UNC\\"
        };

    public:
        static BOOL Create(
            LPCWSTR lpLnkFilePath,
            LPCWSTR lpDstFilePath,
            LPCWSTR pszArguments = NULL,
            LPCWSTR pszDesc = NULL,
            LPCWSTR pszIcon = NULL)
        {
            BOOL bReturn = FALSE;
            ::CoInitialize(NULL);

            if (!lpDstFilePath || !lpLnkFilePath)
                goto Exit0;

            {
                CComPtr<IShellLink> spiShellLink;
                CString strFullPath = lpDstFilePath;
                HRESULT hr = _ExpandFullPathName(strFullPath);
                if (FAILED(hr))
                    goto Exit0;

                hr = ::CoCreateInstance(
                    CLSID_ShellLink,
                    NULL, 
                    CLSCTX_INPROC_SERVER,
                    IID_IShellLink,
                    (void**)&spiShellLink);
                if (FAILED(hr))
                    goto Exit0;

                CComPtr<IPersistFile> spiPersistFile;
                hr = spiShellLink.QueryInterface(&spiPersistFile);
                if (FAILED(hr))
                    goto Exit0;

                // Set the path to the shortcut target and add the description
                hr = spiShellLink->SetPath(strFullPath);
                if (FAILED(hr))
                    goto Exit0;

                if (pszArguments)
                {
                    hr = spiShellLink->SetArguments(pszArguments);
                    if (FAILED(hr))
                        goto Exit0;
                }

                if (pszDesc)
                {
                    hr = spiShellLink->SetDescription(pszDesc);
                    if (FAILED(hr))
                        goto Exit0;
                }

                if (pszIcon)
                {
                    hr = spiShellLink->SetIconLocation(pszIcon, 0);
                    if (FAILED(hr))
                        goto Exit0;
                }

                // Write the shortcut to disk
                hr = spiPersistFile->Save(lpLnkFilePath, TRUE);
                if (FAILED(hr))
                    goto Exit0;
            }

            bReturn = TRUE;

Exit0:
            ::CoUninitialize();
            return bReturn;
        }

        static BOOL GetDstPath(LPCTSTR lpLnkFilePath, CString &sDstPath)
        {
            bool bReturn = FALSE;

            ::CoInitialize(NULL);

            CComPtr<IPersistFile> pPersistFile;
            HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IPersistFile, (void**)(&pPersistFile));
            if(SUCCEEDED(hr))
            {
                CComPtr<IShellLink> pShellLink;
                hr = pPersistFile->QueryInterface(IID_IShellLink, (void**)(&pShellLink));
                if(SUCCEEDED(hr))
                {
                    hr = pPersistFile->Load(lpLnkFilePath, STGM_READ);
                    if(SUCCEEDED(hr))
                    {
                        TCHAR szPath[MAX_PATH + 1] = {0};
                        hr = pShellLink->GetPath(szPath, MAX_PATH, NULL, 0);
                        if(SUCCEEDED(hr))
                        {
                            sDstPath = szPath;
                            bReturn = TRUE;
                        }
                    }
                }
            }

            CoUninitialize();
            return bReturn;
        }

    private:
        static HRESULT _ExpandFullPathName( CString& strPathName )
        {
            DWORD dwLen = ::GetFullPathName(strPathName, 0, NULL, NULL);
            if (0 == dwLen)
                return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

            assert(dwLen <= WIN_PATH_MAX_UNICODE_PATH);
            if (dwLen > WIN_PATH_MAX_UNICODE_PATH)
                return E_UNEXPECTED;

            CString strFullPathName;
            dwLen = ::GetFullPathName(strPathName, dwLen, strFullPathName.GetBuffer(dwLen), NULL);
            if (0 == dwLen)
            {
                strFullPathName.ReleaseBuffer(0);
                return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
            }

            strFullPathName.ReleaseBuffer();
            strPathName = strFullPathName;
            return S_OK;
        }
    };
}
}
