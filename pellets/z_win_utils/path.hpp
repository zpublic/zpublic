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
 * @brief 文件路径操作
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
     /**
     * @brief 提供对windows常见文件路径的操作
     */
    class ZLPath
    {
    public:
        /**
         * @brief 获取模块全路径
         * @param[in] hModule 模块句柄
         * @return 成功返回模块全路径, 失败返回空串
         */
        static CString GetModuleFullPath(HMODULE hModule)
        {
            DWORD dwRet = 0;
            CString strTemp;
            TCHAR szFileName[MAX_PATH + 1] = {0};
            dwRet = ::GetModuleFileName(hModule, szFileName, MAX_PATH);
            if (dwRet)
                strTemp = szFileName;
            return strTemp;
        }
        /**
         * @brief 获取文件名
         * @param[in] szFullPath 文件路径
         * @return 成功返回文件名, 失败返回路径
         */
        static CString PathToFileName(LPCTSTR szFullPath)
        {
            CString strPath(szFullPath);
            strPath.Replace(_T('/'), _T('\\'));
            int nPos = strPath.ReverseFind(_T('\\'));
            if (nPos == -1)
                return strPath;
            else
                return strPath.Right(strPath.GetLength() - nPos - 1);
        }
                
        /**
         * @brief 获取文件夹路径
         * @param[in] szFullPath 文件全路径
         * @return 成功返回文件夹路径名, 失败返回空串
         */
        static CString PathToFolderPath(LPCTSTR szFullPath)
        {
            CString strTemp(szFullPath);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('\\'));
            if (-1 == nPos)
                nPos = strTemp.ReverseFind(_T('/'));
            if (-1 == nPos)
                return _T("");
            return strTemp.Left(nPos + 1);
        }
        /**
         * @brief 获取模块文件夹
         * @param[in] hModule 模块句柄
         * @return 成功返回文件夹路径名, 失败返回空串
         */
        static CString GetModuleFolder(HMODULE hModule)
        {
            return PathToFolderPath(GetModuleFullPath(hModule));
        }
        /**
         * @brief 去除文件后缀
         * @param[in] szFileName 文件名
         * @return 成功返回文件名, 失败返回带后缀的文件名
         */
        static CString FileNameRemoveSuffix(LPCTSTR szFileName)
        {
            CString strTemp(szFileName);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('.'));
            if (-1 == nPos) 
                return strTemp;
            return strTemp.Left(nPos);
        }
        /**
         * @brief 获取路径下的文件后缀
         * @param[in] szFullPath 全路径
         * @return 成功返回文件后缀
         */
        static CString PathToSuffix(LPCTSTR szFullPath)
        {
            CString strTemp = PathToFileName(szFullPath);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('.'));
            if (-1 == nPos) 
                return strTemp;
            return strTemp.Mid(nPos + 1);
        }
        /**
         * @brief 获取父文件夹
         * @param[in] szFullPath 全路径
         * @return 成功返回父文件夹名, 失败返回路径
         */
        static CString GetParsentFolder(LPCTSTR szFullPath)
        {
            CString strParsentPath(szFullPath);
            PathRemoveBackslash(strParsentPath);
            PathRemoveFileSpec(strParsentPath);
            if (strParsentPath.IsEmpty())
                strParsentPath = szFullPath;
            return strParsentPath;
        }
        /**
         * @brief 获取根路径
         * @param[in] szFullPath全路径
         * @return 成功返回根路径, 失败返回全路径
         */
        static CString GetRootPath(LPCTSTR szFullPath)
        {
            CString strRoot(szFullPath);
            CString strTempPath(szFullPath);
            int nPos = strTempPath.Find(_T('\\'));
            if (nPos != -1)
                strRoot = strTempPath.Left(nPos);
            return strRoot;
        }
        /**
         * @brief 获取合法路径
         * @param[in] strPath 路径
         * @return 成功返回路径
         */
        static CString LegalizePath(const CString &strPath)
        {
            CString strOriginPath = strPath;
            CString strData(strOriginPath);
            CString strFolderPath;
            CString strDstPath;
            while (1)
            {
                strFolderPath = GetRootPath(strData);
                if (strFolderPath == _T(".."))
                    strDstPath = GetParsentFolder(strDstPath);
                else
                    strDstPath.Append(strFolderPath);
                PathAddBackslash(strDstPath);
                strData = strData.Mid(strFolderPath.GetLength());
                strData.TrimLeft(L"\\");
                if (strData.IsEmpty())
                    break;
            }
            return strDstPath;
        }
        /**
         * @brief 路径加反斜线
         * @param[in] strPath 路径
         * @return 返回路径
         */
        static void PathAddBackslash(CString& strPath)
        {
            CString strTemp;
            strTemp = strPath.Right(1);
            if (strTemp != _T("\\") && strTemp != _T("/"))
                strPath.Append(_T("\\"));
        }
        /**
         * @brief 去除路径反斜线
         * @param[in] strPath 路径
         * @return 返回去掉反斜线后的路径
         */
        static void PathRemoveBackslash(CString &strPath)
        {
            if (strPath.Right(1) == _T("\\"))
                strPath = strPath.Left(strPath.GetLength() - 1);
        }

        static void PathRemoveFileSpec(CString& strPath)
        {
            int nPos = strPath.ReverseFind(_T('\\'));
            if (nPos == -1)
                strPath.Empty();
            else
                strPath = strPath.Left(nPos);
        }
    };

}
}
