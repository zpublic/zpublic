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
 * @brief 用户SID处理相关
 */


#pragma once
#include "win_utils_header.h"
#include <tlhelp32.h>

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 用户SID操作
     */
    class ZLUsid
    {
    public:
        /**
         * @brief 获取当前用户SID
         * @param[out] 返回用户SID
         * retrun 成功返回TRUE, 失败FALSE
         */
        static BOOL GetCurrentUserSID(CString& strCurrentUserSID)
        {
            BOOL bResult = FALSE;
            LPTSTR lpUserSID = NULL;

            HANDLE hSnapshot = NULL;
            HANDLE hProcess = NULL, hProcessToken = NULL;
            PTOKEN_USER pTokenUser = NULL;

            ULONG ulPid = 0;
            ULONG ulReturnLength = 0;

            PROCESSENTRY32 structProEntry = {0};

            hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (!hSnapshot)
            {goto _abort;}

            structProEntry.dwSize = sizeof(structProEntry);
            if (::Process32First(hSnapshot, &structProEntry))
            {
                do 
                {
                    if (0 == _tcsicmp(_T("explorer.exe"), structProEntry.szExeFile))
                    {
                        ulPid = structProEntry.th32ProcessID;
                        break;
                    }

                } while (::Process32Next(hSnapshot, &structProEntry));
            }

            if (!ulPid)
            {goto _abort;}

            hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ulPid);
            if (!hProcess)
            {goto _abort;}

            if (!::OpenProcessToken(hProcess, TOKEN_QUERY, &hProcessToken))
            {goto _abort;}

            bResult = ::GetTokenInformation(hProcessToken, TokenUser, NULL, 0, &ulReturnLength);
            if (ERROR_INSUFFICIENT_BUFFER != ::GetLastError())
            {goto _abort;}

            pTokenUser = (PTOKEN_USER)HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, ulReturnLength);
            if (!pTokenUser)
            {goto _abort;}

            bResult = ::GetTokenInformation(
                hProcessToken, 
                TokenUser, 
                pTokenUser, 
                ulReturnLength, 
                &ulReturnLength);
            if (!bResult)
            {goto _abort;}

            typedef BOOL (__stdcall *PConvertSidToStringSidW)(PSID Sid, LPTSTR* StringSid);

            if (IsValidSid(pTokenUser->User.Sid))
            {
                PConvertSidToStringSidW pfn_ConvertSidToStringSid = NULL;

                pfn_ConvertSidToStringSid = (PConvertSidToStringSidW)::GetProcAddress(
                    ::LoadLibrary(_T("Advapi32.dll")), 
                    "ConvertSidToStringSidW");
                if (!pfn_ConvertSidToStringSid)
                {goto _abort;}

                bResult = pfn_ConvertSidToStringSid(pTokenUser->User.Sid, &lpUserSID);
                pfn_ConvertSidToStringSid = NULL;

                if (bResult && lpUserSID)
                {
                    strCurrentUserSID = lpUserSID;

                    ::LocalFree(lpUserSID);
                    lpUserSID = NULL;
                }
            }

_abort:

            if (hSnapshot)
            {
                ::CloseHandle(hSnapshot);
                hSnapshot = NULL;
            }

            if (hProcessToken)
            {
                ::CloseHandle(hProcessToken);
                hProcessToken = NULL;
            }

            if (hProcess)
            {
                ::CloseHandle(hProcess);
                hProcess = NULL;
            }

            if (NULL != pTokenUser)
            {
                HeapFree(::GetProcessHeap(), 0, pTokenUser);
                pTokenUser = NULL;
            }

            return bResult;
        }
    };
}
}
