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
    };
}
}
