/********************************************************************
* @file      : singleton.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/21 11:16
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

#include "nocopyable.hpp"

namespace zl
{

    template <class T>
    class singleton : public NoCopyable
    {
    private:
        static T& instance;

    public:
        static T& Instance()
        {
            static T t;
            return t;
        }
    };

    template<class T>
    T& singleton<T>::instance = singleton< T >::Instance();

}

#endif
