/********************************************************************
* @file      : array.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/15 12:59
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

namespace zl
{

    template <typename T, size_t N>
    class CArrayFixed
    {
    public:
        T m_Arr[N];

    public:
        T& operator[](size_t n)
        {
            return m_Arr[n];
        }
        const T& operator[](size_t n) const
        {
            return m_Arr[n];
        }
        size_t Size()
        {
            return N;
        }
    };

}

#endif