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

    namespace ZLMessage
    {
        inline UINT RegisterMessage(LPCTSTR lpMsgName)
        {
            return ::RegisterWindowMessage(lpMsgName);
        }

        typedef BOOL (__stdcall *ChangeWindowMessageFilterType)(UINT, DWORD);
    #define MSGFLT_ADD		1	// ChangeWindowMessageFilter 函数的第二个参数，添加消息
    #define MSGFLT_REMOVE	2	// ChangeWindowMessageFilter 函数的第二个参数，移除消息

        static BOOL ChangeMessageFilter(UINT uMsg, DWORD dwOper = MSGFLT_ADD)
        {
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
    }


}
}
