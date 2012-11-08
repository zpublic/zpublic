/********************************************************************
* @file      : nocopyable.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/08 9:09
* @brief     : √‹∑‚¿‡
* 
* 
*********************************************************************/

#ifndef _NOCOPYABLE_HPP_
#define _NOCOPYABLE_HPP_

namespace zl
{

    class NoCopyable
    {
    private:
        NoCopyable(const NoCopyable&)
        {

        }
        NoCopyable& operator=(const NoCopyable&)
        {
            return *this;
        }
    public:
        NoCopyable()
        {

        }
    };

}

#endif