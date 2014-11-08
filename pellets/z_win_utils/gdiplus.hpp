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
 * @brief gdiplus初始化相关
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief gdiplus初始化相关操作
     */
    class ZLGdiPlusInit
    {
        class Inst
        {
        public:
            Inst():m_bInit(false)
            {

            }
            void Init()
            {
                if (m_bInit)
                {
                    return ;
                }
                Gdiplus::GdiplusStartup(&m_gpToken,  &m_gpInput,  NULL); 
                m_bInit = true;
            }
            void UnInit()
            {
                if (!m_bInit)
                {
                    return ;
                }
                Gdiplus::GdiplusShutdown(m_gpToken);
                m_bInit = false;
            }
        protected:
            bool m_bInit;
            Gdiplus::GdiplusStartupInput m_gpInput; 
            ULONG_PTR           m_gpToken; 
        };
    public:
        ZLGdiPlusInit()
        {
            m_GdiPlusInit.Init();
        }
        ~ZLGdiPlusInit()
        {
            m_GdiPlusInit.UnInit();
        }
    protected:
        Inst m_GdiPlusInit;
    };

}
}
