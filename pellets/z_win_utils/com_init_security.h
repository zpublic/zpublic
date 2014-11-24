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
 * @brief com security初始化相关
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
    * @brief 初始化COM Security
    * 一个进程只能初始化一次
    */
    class ZLComInitSecurity
    {
    public:
        class Inst
        {
        public:
            Inst() : m_bInit(false)
            {

            }
        public:
            BOOL Init()
            {
                if (m_bInit)
                {
                    return TRUE;
                }

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
                    m_bInit = true;
                }
                return m_bInit ? TRUE : FALSE;
            }
            void UnInit()
            {

            }
        protected:
            bool m_bInit;
        };

    public:
        ZLComInitSecurity()
        {
            m_ComInit.Init();
        }

        ~ZLComInitSecurity()
        {
            m_ComInit.UnInit();
        }

    protected:
        ZLComInitSecurity::Inst m_ComInit;
    };
}
}