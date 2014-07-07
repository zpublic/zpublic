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
#include <ShlObj.h>

namespace zl
{
namespace WinUtils
{

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
        HICON GetHandle()
        {
            return m_icon;
        }

    protected:
        HICON m_icon;
    };

}
}
