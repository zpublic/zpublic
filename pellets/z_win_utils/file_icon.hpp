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
 * @brief 文件图标相关
 */


#pragma once
#include "win_utils_header.h"
#include <ShlObj.h>
#include <shellapi.h>

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 文件图标相关
     */
    class ZLFileIcon
    {
    public:
        ZLFileIcon(HICON hIcon = NULL) : m_icon(hIcon) {}
        virtual ~ZLFileIcon() { reset(); }
        void reset()
        {
            if (m_icon != NULL)
            {
                ::DestroyIcon(m_icon);
                m_icon = NULL;
            }
        }

        /**
         * @brief 加载文件图标
         * @param[in] szFilePath 文件路径
         * @param[in] bLargeIcon 是否为大图标，默认TRUE
         * @return 成功返回TRUE，失败返回FALSE
         */
        BOOL Load(LPCTSTR szFilePath, BOOL bLargeIcon = TRUE)
        {
            reset();
            BOOL bRet = FALSE;
            SHFILEINFO fileInfo = {0};
            DWORD dwStyle = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES;
            if (bLargeIcon)
                dwStyle |= SHGFI_LARGEICON;
            else
                dwStyle |= SHGFI_SMALLICON;

            if (::SHGetFileInfo(szFilePath, 0, &fileInfo, sizeof(fileInfo), dwStyle))
            {
                m_icon = fileInfo.hIcon;
                bRet = TRUE;
            }
            return bRet;
        }

        /**
         * @brief 加载文件图标
         * @param[in] szFilePath 文件路径
         * @param[in] uSize      图标的大小
         * @param[in] uIndex     图标下标
         * @return 成功返回TRUE，失败返回FALSE
         */
        BOOL LoadEx(LPCTSTR szFilePath, UINT uSize = 48, UINT uIndex = 0)
        {
            reset();
            if (::SHDefExtractIcon(szFilePath, uIndex, 0, &m_icon, NULL, uSize) == S_OK)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }

        /**
         * @brief 取得图标句柄
         * @return 返回图标的句柄(HICON)
         */
        HICON GetHandle()
        {
            return m_icon;
        }

    protected:
        HICON m_icon;
    };

}
}
