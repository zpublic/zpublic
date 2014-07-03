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

    class Path
    {
    public:
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

        static CString GetModuleFolder(HMODULE hModule)
        {
            return PathToFolderPath(GetModuleFullPath(hModule));
        }

        static CString FileNameRemoveSuffix(LPCTSTR szFileName)
        {
            CString strTemp(szFileName);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('.'));
            if (-1 == nPos) 
                return strTemp;
            return strTemp.Left(nPos);
        }

        static CString PathToSuffix(LPCTSTR szFullPath)
        {
            CString strTemp = PathToFileName(szFullPath);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('.'));
            if (-1 == nPos) 
                return strTemp;
            return strTemp.Mid(nPos + 1);
        }

        static CString GetParsentFolder(LPCTSTR szFullPath)
        {
            CString strParsentPath(szFullPath);
            PathRemoveBackslash(strParsentPath);
            PathRemoveFileSpec(strParsentPath);
            if (strParsentPath.IsEmpty())
                strParsentPath = szFullPath;
            return strParsentPath;
        }

        static CString GetRootPath(LPCTSTR szFullPath)
        {
            CString strRoot(szFullPath);
            CString strTempPath(szFullPath);
            int nPos = strTempPath.Find(_T('\\'));
            if (nPos != -1)
                strRoot = strTempPath.Left(nPos);
            return strRoot;
        }

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

        static void PathAddBackslash(CString& strPath)
        {
            CString strTemp;
            strTemp = strPath.Right(1);
            if (strTemp != _T("\\") && strTemp != _T("/"))
                strPath.Append(_T("\\"));
        }

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
