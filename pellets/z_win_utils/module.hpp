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

    class ZLModule
    {
    public:
        ZLModule() : m_hDllModule(NULL) {}
        ~ZLModule()
        {
            Close();
        }

        void Attach(HMODULE hModule)
        {
            Close();
            m_hDllModule = hModule;
        }

        HMODULE Detach()
        {
            HMODULE hModule = m_hDllModule;
            m_hDllModule = NULL;
            return hModule;
        }

        operator HMODULE()
        {
            return m_hDllModule;
        }
        HMODULE GetModule()
        {
            return m_hDllModule;
        }

        void Close()
        {
            if (m_hDllModule)
            {
                ::FreeLibrary(m_hDllModule);
                m_hDllModule = NULL;
            }
        }
        BOOL Load(LPCTSTR szDllName)
        {
            Close();
            m_hDllModule = ::LoadLibrary(szDllName);
            if (!m_hDllModule)
                return FALSE;
            return TRUE;
        }

        void* GetProc(LPCSTR szFuncName)
        {
            if (!m_hDllModule)
                return NULL;
            return ::GetProcAddress(m_hDllModule, szFuncName);
        }

    private:
        HMODULE m_hDllModule;
    };

}
}
