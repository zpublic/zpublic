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
 * @brief 文件操作相关
 * @todo
 */


#pragma once

#include "win_utils_header.h"
#include "directory.hpp"
#include "buffer.hpp"
#include "file_enum.hpp"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 提供文件操作相关操作
     */
    class ZLFileOperation
    {
    public:
        /**
         * @brief 确保文件所在文件夹存在，若不存在尝试建立
         * @param[in] filePath 文件路径
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL EnsureFileDir(LPCTSTR filePath)
        {
            CString destDir(filePath);
            ZLPath::PathRemoveFileSpec(destDir);
            if (!::PathFileExists(destDir))
            {
                if (!ZLDirectory::CreateDeepDirectory(destDir))
                {
                    return FALSE;
                }
            }
            if (!ZLDirectory::IsDirectory(destDir))
            {
                return FALSE;
            }
            return TRUE;
        }

        /**
         * @brief 拷贝文件
         * @param[in] srcFilePath 源文件路径
         * @param[in] dstFilePath 目标文件路径
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL CopyFile(LPCTSTR srcFilePath, LPCTSTR dstFilePath)
        {
            if (!::PathFileExists(srcFilePath) || !EnsureFileDir(dstFilePath))
            {
                return FALSE;
            }
            return ::CopyFile(srcFilePath,dstFilePath,FALSE);
        }

        /**
         * @brief 通过读写方式拷贝文件
         * @param[in] srcFilePath 源文件路径
         * @param[in] dstFilePath 目标文件路径
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL CopyFileByReadWrite(LPCTSTR srcFilePath, LPCTSTR dstFilePath)
        {
            if (!EnsureFileDir(dstFilePath))
            {
                return FALSE;
            }
            ATL::CHandle hSrcFile(::CreateFile(srcFilePath,GENERIC_READ,FILE_SHARE_READ,
                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));
            ATL::CHandle hDstFile(::CreateFile(dstFilePath,GENERIC_WRITE,0,
                NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
            if (hSrcFile==INVALID_HANDLE_VALUE || hDstFile==INVALID_HANDLE_VALUE)
            {
                return FALSE;
            }
            LARGE_INTEGER fileSizeLI = {0};
            if (!GetFileSizeEx(hSrcFile,&fileSizeLI))
            {
                return FALSE;
            }
            DWORD bufferSize = 4*1024;
            ZLBuffer buffer;
            buffer.Alloc(bufferSize);
            while(fileSizeLI.QuadPart > 0)
            {
                DWORD realReadedSize = 0;
                DWORD realWritedSize = 0;
                if (!ReadFile(hSrcFile, buffer.GetBuffer(), buffer.GetSize(), &realReadedSize, NULL))
                {
                    return FALSE;
                }
                if (!WriteFile(hDstFile, buffer.GetBuffer(), realReadedSize, &realWritedSize, NULL))
                {
                    return FALSE;
                }
                fileSizeLI.QuadPart -= realWritedSize;
            }
            return TRUE;
        }

        /**
         * @brief 移动文件
         * @param[in] srcFilePath 源文件路径
         * @param[in] dstFilePath 目标文件路径
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL MoveFile(LPCTSTR srcFilePath, LPCTSTR dstFilePath)
        {
            if (!::PathFileExists(srcFilePath) || !EnsureFileDir(dstFilePath))
            {
                return FALSE;
            }
            return ::MoveFileEx(srcFilePath, dstFilePath, 
                MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH);
        }

        /**
         * @brief 获取文件只读属性
         * @param[in] filePath 文件路径
         * @return 若文件为只读，返回TRUE，否则返回FALSE
         * @see 
         */
        static BOOL IsFileReadOnly(LPCTSTR filePath)
        {
            DWORD dwAttr = ::GetFileAttributes( filePath );
            if (dwAttr == INVALID_FILE_ATTRIBUTES)
            {
                return FALSE;
            }
            if( dwAttr & FILE_ATTRIBUTE_READONLY )
            {
                return TRUE;
            }
            return FALSE;
        }

        /**
         * @brief 去除文件只读属性
         * @param[in] filePath 文件路径
         * @return 成功返回TRUE，失败返回FALSE，若文件不包含只读属性，也视为成功
         * @see 
         */
        static BOOL RemoveReadOnly(LPCTSTR filePath)
        {
            DWORD dwAttr = ::GetFileAttributes( filePath );
            if (dwAttr == INVALID_FILE_ATTRIBUTES)
            {
                return FALSE;
            }
            if( dwAttr & FILE_ATTRIBUTE_READONLY )
            {
                dwAttr = dwAttr ^ FILE_ATTRIBUTE_READONLY;
                return ::SetFileAttributes( filePath, dwAttr );
            }
            return TRUE;
        }

        /**
         * @brief 重命名文件
         * @param[in] filePath 路径
         * @param[in] newFileName 要重命名的名字，包含文件名后缀
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL RenameFile(LPCTSTR filePath, LPCTSTR newFileName)
        {
            if (!::PathFileExists(filePath))
            {
                return FALSE;
            }
            CString destFile(filePath);
            ZLPath::PathRemoveFileSpec(destFile);
            destFile += newFileName;
            if (::PathFileExists(destFile))
            {
                return FALSE;
            }
            return MoveFile(filePath, destFile);
        }

        /**
         * @brief 寻找未被占用的文件名
         * @param[in,out] filePath 路径
         * @param[in] nameFormat 重命名的格式，%s为文件名，%d为尝试的后缀数字，默认为Windows格式
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL EnsureFileName(CString& filePath,LPCTSTR nameFormat=L"%s_(%d)")
        {
            CString dir(ZLPath::PathToFolderPath(filePath));
            CString fileNameExt(ZLPath::PathToFileName(filePath));
            CString fileName(ZLPath::FileNameRemoveSuffix(fileNameExt));
            CString ext(ZLPath::PathToSuffix(fileNameExt));
            int count = 1;
            while (::PathFileExists(filePath))
            {
                CString newFileName;
                newFileName.Format(nameFormat,fileName, ++count);
                filePath.Format(L"%s\\%s", dir, newFileName);
                if (!ext.IsEmpty())
                {
                    filePath += L'.';
                    filePath += ext;
                }
                if (count == INT_MAX)
                {
                    return FALSE;
                }
            }
            return TRUE;
        }

        /**
         * @brief 删除文件，可选地如果无法立即删除可标记为重启后删除
         * @param[in,out] filePath 路径
         * @param[in,out] rebootDelete 若无法立即删除，是否在重启时删除
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL DeleteFile(LPCTSTR filePath, BOOL rebootDelete)
        {
            if (::DeleteFile(filePath))
            {
                return TRUE;
            }
            if (rebootDelete)
            {
                RemoveReadOnly(filePath);
                if (!::MoveFileEx(filePath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
                {
                    return FALSE;
                }
            }
            return TRUE;
        }

        /**
         * @brief 删除失败会重命名文件并可选地使其在重启后删除
         * @param[in,out] filePath 路径
         * @param[in,out] rebootDelete 若无法立即删除，是否在重启时删除。如果重命名失败，不会尝试标记重启后删除
         * @return 成功返回TRUE，失败返回FALSE
         * @see 
         */
        static BOOL RenameDeleteFile(CString& filePath, BOOL rebootDelete)
        {
            if (::DeleteFile(filePath))
            {
                return TRUE;
            }
            CString newName(filePath);
            if (!EnsureFileName(newName))
            {
                return FALSE;
            }
            RemoveReadOnly(filePath);
            if (!::MoveFileEx(filePath, newName, MOVEFILE_REPLACE_EXISTING))
            {
                return FALSE;
            }
            filePath = newName;
            if (rebootDelete)
            {
                if (!::MoveFileEx(filePath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
                {
                    return FALSE;
                }
            }
            return TRUE;
        }

        /**
         * @brief 删除文件夹及文件.会处理未成功删除的文件
         * @param[in] dir 要删除的目录
         * @param[in] rebootDelete 若无法立即删除，是否在重启时删除。如果重命名失败，不会尝试标记重启后删除
         * @return 无返回值，所有操作视为成功
         * @see 
         */
        static VOID RecursiveDeleteFileDir(LPCTSTR dir, BOOL rebootDelete)
        {
            if (!::PathFileExists(dir))
            {
                return;
            }
            ZLDirectory::DeleteDirectory(dir);
            ZLFileEnum enumer(dir);
            int fileCount = enumer.GetFileCount();
            for (int n=0; n<fileCount; ++n)
            {
                CString fileName(enumer.GetFileName(n));
                RenameDeleteFile(fileName,rebootDelete);
            }
        }
    };
}
}
