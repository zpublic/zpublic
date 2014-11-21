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

                HRESULT hres = ::CoInitialize(NULL);
                if (FAILED(hres))
                {
                    return FALSE;
                }
                m_bInit = true;
                return TRUE;
            }
            void UnInit()
            {
                if (m_bInit)
                {
                    ::CoUninitialize();
                    m_bInit = false;
                }
            }
        protected:
            bool m_bInit;
        };
    public:
        ZLComInit()
        {
            m_ComInit.Init();
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