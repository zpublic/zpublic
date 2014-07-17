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
 * @brief 文件枚举操作
 */


#pragma once
#include "win_utils_header.h"
#include "path.hpp"
#include <vector>

namespace zl
{
namespace WinUtils
{

    typedef struct _ZLFileEnumInfo
    {
        CString m_strFileName;
        DWORD	m_nSize;
    } ZLFileEnumInfo;

    /**
     * @brief 文件枚举相关操作
     */
    class ZLFileEnum
    {
    public:
        /**
         * @brief 初始化
         * @param[in] lpDirName 路径
         * @param[in] lpPost    搜索文件类型，默认为所有文件
         */
        ZLFileEnum(LPCTSTR lpDirName, LPCTSTR lpPost=_T("*.*"))
        {
            if (!lpDirName || !lpPost)
                return;

            CString strSearch(lpDirName);
            ZLPath::PathAddBackslash(strSearch);
            m_strDir = strSearch;
            strSearch.Append(lpPost);

            WIN32_FIND_DATA wfd = {0};
            HANDLE hFind = ::FindFirstFile(strSearch, &wfd);

            if (hFind != INVALID_HANDLE_VALUE)
            {
                do 
                {
                    CString strFileName = wfd.cFileName;
                    if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if (strFileName.Compare(_T("."))==0||
                            strFileName.Compare(_T(".."))==0)
                            continue;
                        m_listDirs.push_back(strFileName);
                    }
                    else
                    {
                        ZLFileEnumInfo fi;
                        fi.m_strFileName = strFileName;
                        fi.m_nSize  = wfd.nFileSizeLow;
                        m_listFiles.push_back(fi);
                    }
                }while (::FindNextFile(hFind, &wfd));
                ::FindClose( hFind );
            }
        }

        /**
         * @brief 获取文件夹下的文件数目
         * @return 返回文件数目
         */
        int GetFileCount()
        {
            return (int)m_listFiles.size();
        }

        /**
         * @brief 获取文件夹下的文件夹数目
         * @return 返回文件夹数目
         */
        int GetDirCount()
        {
            return (int)m_listDirs.size();
        }

        /**
         * @brief 获取指定下标处的文件名
         * @param[in] i 下标
         * @return 文件名
         */
        CString& GetFileName(int i)
        {
            return m_listFiles[i].m_strFileName;
        }

        /**
         * @brief 获取指定下标处文件的大小
         * @param[in] i 下标
         * @return 文件大小
         */
        DWORD GetFilesize(int i)
        {
            return m_listFiles[i].m_nSize;
        }

        /**
         * @brief 获取指定下标处的文件夹名
         * @param[in] i 下标
         * @return 文件夹名
         */
        CString& GetDirName(int i)
        {
            return m_listDirs[i];
        }

        /**
         * @brief 获取指定下标处的文件的全路径
         * @param[in] i 下标
         * @param[out] strFull 文件的全路径
         */
        void GetFileFullPath(int i, CString& strFull)
        {
            strFull = m_strDir;
            strFull.Append(m_listFiles[i].m_strFileName);
        }

        /**
         * @brief 获取指定下标处的文件夹的全路径
         * @param[in] i 下标
         * @param[out] strFull 文件夹的全路径
         */
        void GetDirFullPath(int i, CString& strFull)
        {
            strFull = m_strDir;
            strFull.Append(m_listDirs[i]);
        }

    protected:
        std::vector<ZLFileEnumInfo>     m_listFiles;
        std::vector<CString>            m_listDirs;
        CString                         m_strDir;
    };

}
}
