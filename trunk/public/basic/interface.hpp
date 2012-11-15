/********************************************************************
* @file      : interface.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/08 10:10
* @brief     : ½Ó¿Ú
* 
* 
*********************************************************************/

#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

#include "nocopyable.hpp"

namespace zl
{

    class Interface : public NoCopyable
    {
    public:
        virtual ~Interface()
        {

        }
    };

}

#endif