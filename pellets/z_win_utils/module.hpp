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
 * @brief 动态运行库相关
 */


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
		
	/**
	 * @brief 将可执行模块映射到调用进程的地址空间
	 * @param[in]   szDllName 可执行模块名
	 * @return 如果成功，返回TRUE，如果失败返回FALSE
	 * @see LoadLibrary
	 */
        BOOL Load(LPCTSTR szDllName)
        {
            Close();
            m_hDllModule = ::LoadLibrary(szDllName);
            if (!m_hDllModule)
                return FALSE;
            return TRUE;
        }
	/**
	 * @brief 获取指定函数名的函数地址
	 * @param[in]  szFuncName 以空结尾字符串，包含函数名
	 * @return 如果成功，返回导入库的函数地址，如果失败返回NULL
	 * @see GetProcAddress
	 */
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
