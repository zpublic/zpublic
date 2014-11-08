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

    /**
     * @brief 提供对目录的创建、修改和删除操作
     */
    class ZLDirectory
    {
    public:
        /**
         * @brief 创建目录
         * @param[in] szPath 路径
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL CreateDeepDirectory(LPCTSTR lpPath)
        {
            if (!lpPath)
                return FALSE;

            BOOL bRetCode = FALSE;
            CString strPath(lpPath);
            if (::GetFileAttributes(lpPath) != INVALID_FILE_ATTRIBUTES)
                return TRUE;
            bRetCode = ::CreateDirectory(lpPath, NULL);
            if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
            {
                ZLPath::PathRemoveFileSpec(strPath);
                if (strPath.IsEmpty()) return FALSE;

                bRetCode = CreateDeepDirectory(strPath);
                if (!bRetCode) return FALSE;

                bRetCode = ::CreateDirectory(lpPath, NULL);
                if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
                    return FALSE;
            }
            return TRUE;
        }

        /**
         * @brief 删除目录
         * @param[in] szDir             路径
         * @param[in] bContinueWhenFail 删除某个文件失败时,是否继续
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL DeleteDirectory(LPCTSTR lpDir, BOOL bContinueWhenFail = TRUE)
        {
            if (!lpDir)
                return FALSE;

            BOOL bReturn = FALSE;
            CString sDir(lpDir);
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
         * @param[in] szSrcDir   源目录
         * @param[in] szDstDir   目标目录
         * @param[in] bCoverFile 是否覆盖已存在的文件
         * @return 返回拷贝文件的数目
         * @see 
         */
        static int CopyDirectory(LPCTSTR lpSrcDir, LPCTSTR lpDstDir, BOOL bCoverFile = TRUE)
        {
            if (!lpSrcDir || !lpDstDir)
                return 0;

            int nReturn = 0;
            CString strFind;
            CString strSubFile;
            CString strSubDstFile;
            CString strSrcDir(lpSrcDir);
            CString strDstDir(lpDstDir);
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
