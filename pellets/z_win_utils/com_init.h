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
 * @brief com初始化相关
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
    * @brief 初始化COM
    */
    class ZLComInit
    {
    public:
        class Inst
        {
        public:
            Inst() : m_bInit(FALSE)
            {
            }

        public:
            BOOL Init()
            {
                if (m_bInit == FALSE)
                {
                    HRESULT hres = ::CoInitialize(NULL);
                    if (SUCCEEDED(hres))
                    {
                        m_bInit = TRUE;
                    }
                }
                return m_bInit;
            }

            void UnInit()
            {
                if (m_bInit)
                {
                    ::CoUninitialize();
                    m_bInit = FALSE;
                }
            }

            BOOL InitSecurity()
            {
                BOOL bReturn = FALSE;
                HRESULT hres = ::CoInitializeSecurity(
                    NULL,
                    -1,
                    NULL,
                    NULL,
                    RPC_C_AUTHN_LEVEL_DEFAULT,
                    RPC_C_IMP_LEVEL_IMPERSONATE,
                    NULL,
                    EOAC_NONE,
                    NULL);
                if (SUCCEEDED(hres) || hres == RPC_E_TOO_LATE)
                {
                    bReturn = TRUE;
                }
                return bReturn;
            }

        protected:
            BOOL m_bInit;
        };

    public:
        /**
        * @brief 初始化com
        * @param[in] bSecurity 如果为TRUE 则初始化Security COM
        */
        ZLComInit(BOOL bSecurity = FALSE)
        {
            m_ComInit.Init();
            if (bSecurity == TRUE)
            {
                m_ComInit.InitSecurity();
            }
        }

        ~ZLComInit()
        {
            m_ComInit.UnInit();
        }

    protected:
        ZLComInit::Inst m_ComInit;
    };
}
}
