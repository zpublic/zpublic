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
 * @brief 窗口消息相关
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 窗口消息相关
     */
    class ZLMessage
    {
    public:
        /**
         * @brief 参见MSDN的RegisterWindowMessage函数
         */
        static UINT RegisterMessage(LPCTSTR lpMsgName)
        {
            return ::RegisterWindowMessage(lpMsgName);
        }

        /**
         * @brief 从UIPI中，添加或删除一条消息
         * @param[in] uMsg   要添加The message to add to or remove from the filter.
         * @param[in] dwOper 指定操作类型
         * @return 成功返回TRUE，失败返回FALSE
         * @see ChangeWindowMessageFilter
         */
        static BOOL ChangeMessageFilter(UINT uMsg, DWORD dwOper = 1 /* MSGFLT_ADD */ )
        {
            typedef BOOL (__stdcall *ChangeWindowMessageFilterType)(UINT, DWORD);

            BOOL bRet = FALSE;
            static HMODULE hModule = NULL;
            static ChangeWindowMessageFilterType pChangeWindowMessageFilterType = NULL;
            if (pChangeWindowMessageFilterType == NULL)
            {
                hModule = ::LoadLibrary(TEXT("user32.dll"));
                if (hModule == NULL)
                    goto Exit0;
                pChangeWindowMessageFilterType = (ChangeWindowMessageFilterType)::GetProcAddress(hModule, "ChangeWindowMessageFilter");
                if (pChangeWindowMessageFilterType == NULL)
                    goto Exit0;
            }
            bRet = pChangeWindowMessageFilterType(uMsg, dwOper);
    Exit0:
            return bRet;
        }
    };


}
}
