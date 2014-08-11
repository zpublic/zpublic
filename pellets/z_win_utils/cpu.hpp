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
 * @brief cpu信息
 */


#pragma once
#include "win_utils_header.h"
#include "register.hpp"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief cpu信息相关
     */
    class ZLCpu
    {
    public:
        /**
         * @brief 获取处理器个数
         * @return 返回处理器个数
         */
        static int GetProcessorsCount()
        {
            SYSTEM_INFO info = {0};
            ::GetSystemInfo(&info);
            return info.dwNumberOfProcessors;
        }

        /**
         * @brief 获取处理器主频
         * @return 返回处理器主频，以MHz为单位
         */
        static DWORD GetCpuMHz()
        {
            DWORD dwMHz = 0;
            ZLRegister reg;
            if (reg.Open(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"))
            {
                reg.GetDwordValue(L"~MHz", dwMHz);
            }
            return dwMHz;
        }
    };

}
}
