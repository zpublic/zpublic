/********************************************************************
* @file      : allocer.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/14 17:56
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _ALLOCER_HPP_
#define _ALLOCER_HPP_

#include <stdlib.h>

namespace zl
{

    template <typename _Ty, typename _SizeType> inline
    _Ty *_Allocate(_SizeType _Count, _Ty*)
    {
        void *_Ptr = 0;
        if (_Count != 0)
            _Ptr = malloc(_Count);

        return ((_Ty *)_Ptr);
    }

    template <typename _Ty, typename _SizeType> inline
    _Ty *_Allocate(_Ty*, _SizeType _Count)
    {
        void *_Ptr = 0;
        if (_Count != 0)
            _Ptr = malloc(_Count * sizeof(_Ty));

        return ((_Ty *)_Ptr);
    }

    template <typename _Ty, typename _SizeType> inline
    _Ty *_Allocate(_SizeType _Count)
    {
        void *_Ptr = 0;
        if (_Count != 0)
            _Ptr = malloc(_Count * sizeof(_Ty));

        return ((_Ty *)_Ptr);
    }

    template <typename _SizeType, typename _Ty> inline
    _Ty *_ReAllocate(_SizeType _Count, _Ty* _PtrOld)
    {
        void *_Ptr = 0;
        if (_Count != 0)
            _Ptr = realloc(_PtrOld, _Count);

        return ((_Ty *)_Ptr);
    }

    template <typename _Ty> inline
    void _Free(_Ty* _Ptr)
    {
        free(_Ptr);
    }

}

#endif