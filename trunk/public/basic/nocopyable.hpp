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

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&)
#endif

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