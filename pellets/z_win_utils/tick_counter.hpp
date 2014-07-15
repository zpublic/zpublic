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
 * @brief 计时器
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    typedef struct _ZL_TICK_COUNTER
    {
        double useTime;
        double allTime;
        int    nPause;
    }ZL_TICK_COUNTER;

    /**
     * @brief 计时器操作
     * @par Example:
     * @code
     * 
     * @endcode
     */
    class ZLTickCounter
    {
    public:
        ZLTickCounter()
        {
            reset();
            //获取每秒多少CPU Performance Tick  
            ::QueryPerformanceFrequency(&m_liFreq);
        }

        /**
         * @brief 开始计时
         */
        void Start()
        {
            reset();
            ::QueryPerformanceCounter(&m_StartTime); 
            Entry();
        }

        /**
         * @brief 停止计时,返回计时时间、总时间、暂停计时的次数（时间单位毫秒）
         */
        void Stop(ZL_TICK_COUNTER& tickCounterInfo)
        {
            Leave();
            LARGE_INTEGER liPerfNow = {0};
            QueryPerformanceCounter(&liPerfNow);
            m_TickCount = ((liPerfNow.QuadPart - m_StartTime.QuadPart) * 1000.0) / m_liFreq.QuadPart; 
            tickCounterInfo.useTime = m_EffectTickCount;
            tickCounterInfo.allTime = m_TickCount;
            tickCounterInfo.nPause  = m_nLeaveTimes - 1;
        }

        // 暂停计时
        void Leave()
        {
            LARGE_INTEGER liPerfNow = {0};
            ::QueryPerformanceCounter(&liPerfNow);
            m_EffectTickCount += ((liPerfNow.QuadPart - m_EntryTime.QuadPart) * 1000.0) / m_liFreq.QuadPart; 
            m_nLeaveTimes++;
        }

        // 继续计时
        void Entry()
        {
            ::QueryPerformanceCounter(&m_EntryTime);
        }

    private:
        void reset()
        {
            m_TickCount = 0.0;
            m_EffectTickCount = 0.0;
            m_nLeaveTimes = 0;
            m_StartTime.QuadPart = 0;
            m_EntryTime.QuadPart = 0;
        }

        double m_TickCount;
        double m_EffectTickCount;
        LARGE_INTEGER m_StartTime;
        LARGE_INTEGER m_EntryTime;
        LARGE_INTEGER m_liFreq;
        int m_nLeaveTimes;
    };

}
}
