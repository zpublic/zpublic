/********************************************************************
* @file      : time_interval.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/19 21:25
* @brief     : 使用QueryPerformanceCounter计时，高精度
* 
* 
*********************************************************************/

#ifndef _TIME_INTERVAL_HPP_
#define _TIME_INTERVAL_HPP_

namespace zl
{

    class CTimeInterval
    {
    private:
        CTimeInterval();
        ~CTimeInterval();

    public:

        static BOOL CTimeInterval::Calc( LARGE_INTEGER& llTimeBegin, double& dfTimeInterval )
        {
            LARGE_INTEGER timeEnd = { 0 };
            if (llTimeBegin.QuadPart != 0
                && ::QueryPerformanceFrequency(&m_Freq)
                && ::QueryPerformanceCounter(&timeEnd))
            {
                dfTimeInterval = double(timeEnd.QuadPart - llTimeBegin.QuadPart) / m_Freq.QuadPart * 1000.0;
                return TRUE;
            }
            return FALSE;
        }

        static BOOL CTimeInterval::GetTime( LARGE_INTEGER& llCurrentTime )
        {
            return ::QueryPerformanceCounter(&llCurrentTime);
        }

    private:
        static LARGE_INTEGER m_Freq;
    };

    LARGE_INTEGER CTimeInterval::m_Freq = { 0 };
}

#endif