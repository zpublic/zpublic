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
 * @brief 任务栏位置
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{

    enum emTaskbarPosition
    {
        TaskballPositionInvaild,
        TaskballPositionLeft,
        TaskballPositionRight,
        TaskballPositionTop,
        TaskballPositionBottom,
        TaskballPositionHide
    };
        /**
         * @brief 获取任务栏的位置
         * @return 成功返回任务栏的位置
         * @see SystemParametersInfo
         */
    static emTaskbarPosition GetTaskbarPosition()
    {
        RECT rcWA;
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWA, 0);
        if (rcWA.left > 0)
        {
            return TaskballPositionLeft;
        }
        else if (rcWA.top > 0)
        {
            return TaskballPositionTop;
        }
        else
        {
            int iScrWidth = ::GetSystemMetrics(SM_CXSCREEN);
            int iScrHeight = ::GetSystemMetrics(SM_CYSCREEN);
            if (rcWA.bottom - rcWA.top < iScrHeight)
            {
                return TaskballPositionBottom;
            }
            else if (rcWA.right - rcWA.left < iScrWidth)
            {
                return TaskballPositionRight;
            }
            else
            {
                return TaskballPositionHide;
            }
        }
    }

}
}
