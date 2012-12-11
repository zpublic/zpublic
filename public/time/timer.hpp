/********************************************************************
* @file      : timer.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/21 11:39
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include <time.h>

namespace zl
{
    const double timer_elapsed_min = double(1) / double(CLOCKS_PER_SEC);

    class timer
    {
    public:
        timer()
        {
            _start_time = clock();
        }

        void restart()
        {
            _start_time = clock();
        }

        double elapsed() const
        {
            return  double(clock() - _start_time) / CLOCKS_PER_SEC;
        }

    private:
        clock_t _start_time;
    };

}

#endif