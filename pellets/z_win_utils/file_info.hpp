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
 * @brief 文件信息相关
 * 
 *      文件信息包括大小,修改时间等.
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 文件信息相关函数集合
     */
    class ZLFileInfo
    {
    public:

        /**
         * @brief 获取文件大小
         * @param[in] lpFilePath 文件路径
         * @param[out] lSize     用于存储文件的大小信息
         * @return 成功返回TRUE, 参数lSize中存放的是文件的大小信息, 失败返回FALSE.
         */
        static BOOL GetFileSize(LPCTSTR lpFilePath, LONGLONG &lSize)
        {
            BOOL bReturn = FALSE;

            HANDLE hFile = ::CreateFile(
                lpFilePath,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE |FILE_SHARE_DELETE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

            if (INVALID_HANDLE_VALUE != hFile)
            {
                LARGE_INTEGER lFileSize = {0};
                if (::GetFileSizeEx(hFile, &lFileSize))
                {
                    lSize = lFileSize.QuadPart;
                    bReturn = TRUE;
                }

                ::CloseHandle(hFile);
            }

            return bReturn;
        }

        /**
         * @brief 获取文件的创建时间,访问时间及修改时间
         * @param[in]  lpFilePath   文件路径
         * @param[out] pftCreate    用于存储得到的创建时间,不需要可填NULL
         * @param[out] pftAccess    用于存储得到的访问时间,不需要可填NULL
         * @param[out] pftWrite     用于存储得到的修改时间,不需要可填NULL
         * @return 成功返回TRUE,失败返回FALSE
         */
        static BOOL GetFileTimeInfo(
            LPCTSTR   lpFilePath,
            PFILETIME pftCreate,
            PFILETIME pftAccess,
            PFILETIME pftWrite)
        {
            BOOL bReturn = FALSE;

            HANDLE hFile = ::CreateFile(
                lpFilePath,
                GENERIC_READ, 
                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
                NULL, 
                OPEN_EXISTING, 
                FILE_ATTRIBUTE_NORMAL, 
                NULL);

            if (hFile != INVALID_HANDLE_VALUE)
            {
                bReturn = ::GetFileTime(hFile, pftCreate, pftAccess, pftWrite);

                ::CloseHandle(hFile);
            }

            return bReturn;
        }

        /**
         * @brief 文件信息类
         * @author Manuel Laflamme
         */
        class ZLFileVersion
        {
        public:
            ZLFileVersion()
            {
                m_lpVersionData = NULL;
                m_dwLangCharset = 0;
            }
            ~ZLFileVersion()
            {
                Close();
            }
        public:
            /**
             * @brief 获取文件的信息
             * @param[in]  lpFilePath   文件路径
             * @return 成功返回TRUE,失败返回FALSE
             */
            BOOL Open(LPCTSTR lpszModuleName)
            {
                if (m_lpVersionData || !lpszModuleName)
                {
                    return FALSE;
                }

                DWORD dwHandle;
                DWORD dwDataSize = ::GetFileVersionInfoSize((LPTSTR)lpszModuleName, &dwHandle);
                if ( dwDataSize == 0 )
                    return FALSE;

                m_lpVersionData = new BYTE[dwDataSize];
                if (!::GetFileVersionInfo((LPTSTR)lpszModuleName, dwHandle, dwDataSize,
                    (void**)m_lpVersionData) )
                {
                    Close();
                    return FALSE;
                }

                UINT nQuerySize;
                DWORD* pTransTable;
                if (!::VerQueryValue(m_lpVersionData, _T("\\VarFileInfo\\Translation"),
                    (void **)&pTransTable, &nQuerySize) )
                {
                    Close();
                    return FALSE;
                }

                m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
                return TRUE;
            }

            /**
             * @brief 关闭文件
             */
            void Close()
            {
                if (m_lpVersionData)
                {
                    delete[] m_lpVersionData;
                    m_lpVersionData = NULL;
                }
                m_dwLangCharset = 0;
            }
            
            /**
             * @brief 获取指定信息，参见MSDN对VerQueryValue的描述
             * @param[in]  lpszValueName   获取的信息名
             * @param[in]  dwLangCharset   语言ID
             * @return 返回查询到的字符串信息
             */
            CString QueryValue(LPCTSTR lpszValueName, DWORD dwLangCharset = 0)
            {
                if ( m_lpVersionData == NULL )
                    return (CString)_T("");

                if ( dwLangCharset == 0 )
                    dwLangCharset = m_dwLangCharset;

                UINT nQuerySize;
                LPVOID lpData;
                CString strValue, strBlockName;
                strBlockName.Format(_T("\\StringFileInfo\\%08lx\\%s"),
                    dwLangCharset, lpszValueName);
                if ( ::VerQueryValue((void **)m_lpVersionData, strBlockName.GetBuffer(),
                    &lpData, &nQuerySize) )
                    strValue = (LPCTSTR)lpData;
                strBlockName.ReleaseBuffer();
                return strValue;
            }

            CString GetFileDescription() {return QueryValue(_T("FileDescription")); };
            CString GetFileVersion() {return QueryValue(_T("FileVersion")); };
            CString GetInternalName() {return QueryValue(_T("InternalName")); };
            CString GetCompanyName() {return QueryValue(_T("CompanyName")); };
            CString GetLegalCopyright() {return QueryValue(_T("LegalCopyright")); };
            CString GetOriginalFilename() {return QueryValue(_T("OriginalFilename"));};
            CString GetProductName() {return QueryValue(_T("ProductName")); };
            CString GetProductVersion() {return QueryValue(_T("ProductVersion")); };
            
            /**
             * @brief 获取VS_FIXEDFILEINFO信息
             * @param[out]  vsffi   信息结构体
             * @return 成功返回TRUE,失败返回FALSE
             */
            BOOL GetFixedInfo(VS_FIXEDFILEINFO& vsffi)
            {
                if ( m_lpVersionData == NULL )
                    return FALSE;
                UINT nQuerySize;
                VS_FIXEDFILEINFO* pVsffi;
                if ( ::VerQueryValue((void **)m_lpVersionData, _T("\\"),
                    (void**)&pVsffi, &nQuerySize) )
                {
                    vsffi = *pVsffi;
                    return TRUE;
                }
                return FALSE;
            }
        protected:
            LPBYTE m_lpVersionData;
            DWORD m_dwLangCharset;
        };
    };
}
}
