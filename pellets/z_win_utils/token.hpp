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
 * @brief 进程权限令牌相关
 */


#pragma once
#include "win_utils_header.h"
#include <tlhelp32.h>
#include <vector>

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 进程权限令牌相关
     */
    class ZLToken
    {
    public:
        ZLToken()
        {
            m_bChangeSucceed = FALSE;
            m_hExplorerToken = NULL;
        }

        ~ZLToken()
        {
            if (m_hExplorerToken)
            {
                ::CloseHandle(m_hExplorerToken);
                m_hExplorerToken = NULL;
            }
        }

        BOOL IncreasePrivilege()
        {
            if (m_bChangeSucceed)
                return RevertToSelf();
            else
                return FALSE;
        }

        BOOL DecreasePrivilege()
        {
            HANDLE hUserToken = GetExplorerToken();
            if (hUserToken)
            {
                m_bChangeSucceed = ImpersonateLoggedOnUser(hUserToken);
            }	
            else
            {
                m_bChangeSucceed = FALSE;
            }
            return m_bChangeSucceed;
        }

        /**
         * @brief 获取浏览器Token
         * @return 成功返回浏览器Token句柄，失败返回NULL
         * @see 
         * 
         */
        HANDLE GetExplorerToken()
        {
            if (m_hExplorerToken != NULL)
            {
                return m_hExplorerToken;
            }
            typedef DWORD (WINAPI *WTSGetActiveConsoleSessionIdPROC)();
            WTSGetActiveConsoleSessionIdPROC WTSGetActiveConsoleSessionId = NULL;
            DWORD dwCurrentSectionId = 0;
            bool bLoad = true;
            HMODULE hInstKernel32 = LoadLibrary(_T("Kernel32.dll"));
            if (!hInstKernel32)
            {
                bLoad = false;
            }
            if (bLoad)
            {
                WTSGetActiveConsoleSessionId = (WTSGetActiveConsoleSessionIdPROC)::GetProcAddress(hInstKernel32, "WTSGetActiveConsoleSessionId");

                if (!WTSGetActiveConsoleSessionId)
                {
                    bLoad = false;
                }
            }
            if (bLoad)
            {
                dwCurrentSectionId = WTSGetActiveConsoleSessionId();
                if (0xFFFFFFFF == dwCurrentSectionId)
                {
                    bLoad = false;
                }
            }
            if (hInstKernel32)
            {
                ::FreeLibrary(hInstKernel32);
                hInstKernel32 = NULL;
            }
            if (_FindExplorer() && bLoad)
            {
                typedef std::vector<DWORD>::iterator HandleVistor;
                for (HandleVistor iterBegin = m_ExplorerVec.begin(); iterBegin != m_ExplorerVec.end();
                    ++iterBegin)
                {
                    DWORD dwProcessID = (*iterBegin);
                    DWORD dwSectionid = 0;
                    ::ProcessIdToSessionId(dwProcessID, &dwSectionid);
                    if (dwCurrentSectionId == dwSectionid)
                    {
                        HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
                        BOOL bRet = ::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &m_hExplorerToken);
                        if (hProcess)
                        {
                            ::CloseHandle(hProcess);
                            hProcess = NULL;
                        }
                        if (bRet)
                        {
                            return  m_hExplorerToken;
                        }
                    }
                }
                // 到这里表明没有相同的， 那么只能任意一个了
                HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_ExplorerVec[0]);
                BOOL bRet = ::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &m_hExplorerToken);

                if (hProcess)
                {
                    ::CloseHandle(hProcess);
                    hProcess = NULL;
                }
                if (bRet)
                {

                    return  m_hExplorerToken;
                }
            }
            else if (!m_ExplorerVec.empty() && !bLoad)
            {
                HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_ExplorerVec[0]);
                BOOL bRet = ::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &m_hExplorerToken);
                if (hProcess)
                {
                    ::CloseHandle(hProcess);
                    hProcess = NULL;
                }
                if (bRet)
                {
                    return  m_hExplorerToken;
                }
            }
            return NULL;
        }

    private:
        BOOL _FindExplorer()
        {
            m_ExplorerVec.clear();
            HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,  0);
            if (INVALID_HANDLE_VALUE  == hSnapshot)
            {
                return FALSE;
            }
            PROCESSENTRY32 pe32;
            pe32.dwSize = sizeof( PROCESSENTRY32 );
            if( !::Process32First(hSnapshot, &pe32 ) )
            {
                if (hSnapshot)
                {
                    ::CloseHandle( hSnapshot );
                    hSnapshot = NULL;
                }
                return FALSE;
            }
            do
            {
                if (_tcsicmp(pe32.szExeFile, _T("explorer.exe")) == 0)
                {
                    m_ExplorerVec.push_back(pe32.th32ProcessID);
                }
            } while( ::Process32Next( hSnapshot, &pe32 ) );
            if (hSnapshot)
            {
                ::CloseHandle(hSnapshot);
                hSnapshot = NULL;
            }
            return !m_ExplorerVec.empty();
        }

        BOOL                m_bChangeSucceed;
        HANDLE              m_hExplorerToken;
        std::vector<DWORD>  m_ExplorerVec;
    };

}
}
