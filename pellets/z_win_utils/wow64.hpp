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
 * @brief 32位、64位兼容处理
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    typedef 
        BOOL
        WINAPI
        Fn_Wow64DisableWow64FsRedirectionFunction(
        __out PVOID *OldValue
        );

    typedef 
        BOOL
        WINAPI
        Fn_Wow64RevertWow64FsRedirectionFunction(
        __in PVOID OlValue
        );

    typedef BOOL
        WINAPI
        Fn_IsWow64Process(
        __in  HANDLE hProcess,
        __out PBOOL Wow64Process
        );
    /**
     * @brief 32位、64位兼容处理
     */
    class ZLWow64
    {
    public:
        ZLWow64() {}
        ~ZLWow64() {}

    public:
        /**
         * @brief 进程是否运行在Wow64下
         * @param[in]  hProcess       进程句柄
         * @param[out] pbWow64Process BOOL指针
         * @return 成功返回TRUE，失败返回FALSE
         * @see IsWow64Process
         */
        static BOOL CheckIsWow64Process(HANDLE hProcess, BOOL* pbWow64Process)
        {
            static Fn_IsWow64Process* s_pfnIsWow64 = (Fn_IsWow64Process*)::GetProcAddress(
                ::GetModuleHandle(_T("kernel32.dll")),
                "IsWow64Process");

            return s_pfnIsWow64 ? s_pfnIsWow64(hProcess, pbWow64Process) : FALSE;
        }
        /**
         * @brief 当前进程是否运行在Wow64下
         * @param[out] pbWow64Process BOOL指针
         * @return 成功返回TRUE，失败返回FALSE
         */
        static BOOL CheckCurrentProcessIsWow64Process(BOOL* pbWow64Process)
        {
            static BOOL s_bIsWow64Process = FALSE;
            static BOOL s_bResult = CheckIsWow64Process(::GetCurrentProcess(), &s_bIsWow64Process);

            if (pbWow64Process)
            {
                *pbWow64Process = s_bIsWow64Process;
            }
            return s_bResult;
        }
        /**
         * @brief 禁用文件系统重定向为调用线程
         * @param[out] ppVoidValue Wow64文件系统重定向值
         * @return 成功返回TRUE，失败返回FALSE
         */
        static BOOL Wow64DisableWow64FsRedirection(PVOID* ppVoidValue)
        {
            static Fn_Wow64DisableWow64FsRedirectionFunction* s_pfnWow64DisableWow64FsRedirection = 
                (Fn_Wow64DisableWow64FsRedirectionFunction*)::GetProcAddress(
                ::GetModuleHandle(_T("kernel32.dll")),
                "Wow64DisableWow64FsRedirection");

            return s_pfnWow64DisableWow64FsRedirection ? s_pfnWow64DisableWow64FsRedirection(ppVoidValue) : FALSE;
        }
        /**
         * @brief 恢复文件系统重定向为调用线程
         * @param[in] ppVoidValue Wow64文件系统重定向值
         * @return 成功返回TRUE，失败返回FALSE
         */
        static BOOL Wow64RevertWow64FsRedirection(PVOID pOldValue)
        {
            static Fn_Wow64RevertWow64FsRedirectionFunction* s_pfnWow64RevertWow64FsRedirection = 
                (Fn_Wow64RevertWow64FsRedirectionFunction*)::GetProcAddress(
                ::GetModuleHandle(_T("kernel32.dll")), 
                "Wow64RevertWow64FsRedirection");

            return s_pfnWow64RevertWow64FsRedirection ? s_pfnWow64RevertWow64FsRedirection(pOldValue) : FALSE;
        }
    };

    class ZLWow64Guard
    {
    public:
        ZLWow64Guard()
            : bIsWow64_(FALSE)
            , pVoidValue_(NULL)
        {
            if (ZLWow64::CheckCurrentProcessIsWow64Process(&bIsWow64_) && bIsWow64_)
            {
                ZLWow64::Wow64DisableWow64FsRedirection(&pVoidValue_);
            }
        }

        ~ZLWow64Guard()
        {
            if (bIsWow64_)
            {
                ZLWow64::Wow64RevertWow64FsRedirection(pVoidValue_);
            }
        }

    private:
        BOOL  bIsWow64_;
        PVOID pVoidValue_;
    };
}
}