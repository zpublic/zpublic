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
#include "path.hpp"

namespace zl
{
namespace WinUtils
{

    /*
     *  CreateDeepDirectory     创建多层目录
     *  DeleteDirectory         删除目录
     *  CopyDirectory           拷贝目录
     */
    class Directory
    {
    public:
        static BOOL CreateDeepDirectory(LPCTSTR szPath)
        {
            BOOL bRetCode = FALSE;
            CString strPath(szPath);
            if (::GetFileAttributes(szPath) != INVALID_FILE_ATTRIBUTES)
                return TRUE;
            bRetCode = ::CreateDirectory(szPath, NULL);
            if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
            {
                Path::PathRemoveFileSpec(strPath);
                if (strPath.IsEmpty()) return FALSE;

                bRetCode = CreateDeepDirectory(strPath);
                if (!bRetCode) return FALSE;

                bRetCode = ::CreateDirectory(szPath, NULL);
                if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
                    return FALSE;
            }
            return TRUE;
        }

        static BOOL DeleteDirectory(LPCTSTR szDir, BOOL bContinueWhenFail = TRUE)
        {
            BOOL bReturn = FALSE;
            CString sDir(szDir);
            CString sFindPath;
            WIN32_FIND_DATA fData;
            HANDLE hFind = INVALID_HANDLE_VALUE;
            Path::PathAddBackslash(sDir);
            sFindPath.Format(_T("%s*.*"), sDir);
            hFind = ::FindFirstFile(sFindPath, &fData);
            if (hFind == INVALID_HANDLE_VALUE)
                goto Exit0;
            do 
            {
                if (0 == _tcscmp(fData.cFileName, _T(".")) ||
                    0 == _tcscmp(fData.cFileName, _T("..")))
                    continue;
                if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    CString sSubfolder;
                    sSubfolder.Format(_T("%s%s\\"), sDir, fData.cFileName);
                    if ((FALSE == DeleteDirectory(sSubfolder, bContinueWhenFail)) && (!bContinueWhenFail))
                        goto Exit0;
                }
                else 
                {
                    CString sFileName = fData.cFileName;
                    sFileName.MakeLower();
                    if ((FALSE == ::DeleteFile(sDir + sFileName)) && (!bContinueWhenFail))
                        goto Exit0;
                }
            } while (::FindNextFile(hFind, &fData) != 0);
            bReturn = TRUE;
Exit0:
            if (hFind != INVALID_HANDLE_VALUE)
                ::FindClose(hFind);
            ::RemoveDirectory(sDir);
            Path::PathRemoveBackslash(sDir);
            ::RemoveDirectory(sDir);

            return bReturn;
        }

        static int CopyDirectory(LPCTSTR szSrcDir, LPCTSTR szDstDir, BOOL bCoverFile = TRUE)
        {
            int nReturn = 0;
            CString strFind;
            CString strSubFile;
            CString strSubDstFile;
            CString strSrcDir(szSrcDir);
            CString strDstDir(szDstDir);
            WIN32_FIND_DATA FindFileData;
            HANDLE hFind = INVALID_HANDLE_VALUE;
            Path::PathAddBackslash(strSrcDir);
            Path::PathAddBackslash(strDstDir);
            CreateDeepDirectory(strDstDir);
            strFind.Format(_T("%s*.*"), strSrcDir);
            hFind = ::FindFirstFile(strFind, &FindFileData);
            if (hFind == INVALID_HANDLE_VALUE)
                goto Exit0;
            do 
            {
                if (_tcscmp(FindFileData.cFileName, _T(".")) == 0
                    || _tcscmp(FindFileData.cFileName, _T("..")) == 0)
                    continue;

                if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    strSubFile.Format(_T("%s%s\\"), strSrcDir, FindFileData.cFileName);
                    strSubDstFile.Format(_T("%s%s\\"), strDstDir, FindFileData.cFileName);

                    CreateDeepDirectory(strSubDstFile);
                    nReturn += CopyDirectory(strSubFile, strSubDstFile, !bCoverFile);
                }
                else
                {
                    strSubFile.Format(_T("%s%s"), strSrcDir, FindFileData.cFileName);
                    strSubDstFile.Format(_T("%s%s"), strDstDir, FindFileData.cFileName);

                    if (!::CopyFile(strSubFile, strSubDstFile, !bCoverFile))
                        nReturn++;
                }
            } while (::FindNextFile(hFind, &FindFileData) != 0);
Exit0:
            if (hFind != INVALID_HANDLE_VALUE)
                ::FindClose(hFind);
            return nReturn;
        }
    };

}
}
