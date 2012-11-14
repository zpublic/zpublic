/********************************************************************
* @file      : increase_policy.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/14 15:32
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _INCREASE_POLICY_
#define _INCREASE_POLICY_

namespace zl
{

    template <unsigned long N>
    class PIncreaseRegular
    {
    public:
        static unsigned long Increase(unsigned long num)
        {
            num += N;
            return num;
        }
    };

    template <unsigned long N>
    class PIncreaseMul
    {
    public:
        static unsigned long Increase(unsigned long num)
        {
            num *= N;
            return num;
        }
    };

}

#endif