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
 * @brief 目录/文件夹相关
 */


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
    class ZLDirectory
    {
    public:
        /**
         * @brief 创建目录
         * @param[in] szPath 路径名
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL CreateDeepDirectory(LPCTSTR szPath)
        {
            BOOL bRetCode = FALSE;
            CString strPath(szPath);
            if (::GetFileAttributes(szPath) != INVALID_FILE_ATTRIBUTES)
                return TRUE;
            bRetCode = ::CreateDirectory(szPath, NULL);
            if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
            {
                ZLPath::PathRemoveFileSpec(strPath);
                if (strPath.IsEmpty()) return FALSE;

                bRetCode = CreateDeepDirectory(strPath);
                if (!bRetCode) return FALSE;

                bRetCode = ::CreateDirectory(szPath, NULL);
                if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
                    return FALSE;
            }
            return TRUE;
        }
        /**
         * @brief 删除指定目录
         * @param[in] szPath            路径名
         * @param[in] bContinueWhenFail 控制删除目录失败时的行为，默认为TRUE
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL DeleteDirectory(LPCTSTR szDir, BOOL bContinueWhenFail = TRUE)
        {
            BOOL bReturn = FALSE;
            CString sDir(szDir);
            CString sFindPath;
            WIN32_FIND_DATA fData;
            HANDLE hFind = INVALID_HANDLE_VALUE;
            ZLPath::PathAddBackslash(sDir);
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
            ZLPath::PathRemoveBackslash(sDir);
            ::RemoveDirectory(sDir);

            return bReturn;
        }
        /**
         * @brief 拷贝目录
         * @param[in] szSrcDir   源目录名
         * @param[in] szDstDir   目的目录名
         * @param[in] bCoverFile 控制函数的行为，默认为TRUE
         * @return 返回拷贝文件的数目
         * @see 
         */
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
            ZLPath::PathAddBackslash(strSrcDir);
            ZLPath::PathAddBackslash(strDstDir);
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
