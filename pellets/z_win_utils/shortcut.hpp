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
 * @brief 快捷方式处理
 */

#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 快捷方式处理
     * 用于生成给定文件的快捷方式，由快捷方式获取文件路径与文件名
     */
    class ZLShortcut
    {

        enum
        {
            WIN_PATH_MAX_UNICODE_PATH           = 32767 + 4,    ///> max size of buffer to store path, in bytes
            WIN_PATH_UNICODE_PATH_PREFIX        = 4,            ///> length of "\\\\??\\"
            WIN_PATH_UNICODE_UNC_PATH_PREFIX    = 7,            ///> length of "\\\\??\\UNC\\"
        };

    public:
        /**
         * @brief 创建可执行文件的快捷方式
         * @param[in] lpLnkFilePath 快捷方式的路径与文件名
         * @param[in] lpDstFilePath 可执行文件路径与文件名
         * @param[in] pszArguments  命令行参数
         * @param[in] pszDesc       描述信息
         * @param[in] pszIcon       图标
         * @return 成功，返回TRUE，失败返回FALSE
         * @see _ExpandFullPathName
         */
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
		
        /**
         * @brief 获取快捷方式对应的路径和文件名
         * @param[in]  lpLnkFilePath 字符串包含打开文件的绝对路径
         * @param[out] sDstPath      获取快捷方式的路径和文件名
         * @return 成功，返回TRUE，失败返回FALSE
         */
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
	
        /**
         * @brief 由指定文件名获取文件的全路径与文件名
         * @param[out] strPathName 返回文件全路径与文件名
         * @return 如果成功，返回S_OK，如果失败，以HRESULT的形式返回调用线程的最近错误码或E_FAIL
         * @see GetFullPathName
         */
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


