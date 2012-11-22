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

//#include <stdlib.h>

/*
* 2012-11-22
* kevin
* 因为对象构造的问题，将空间配置修改成new,添加construct 和 destory函数.
*/
namespace zl
{

    template <typename _Ty, typename _SizeType> inline
    _Ty *_Allocate(_SizeType _Count, _Ty*)
    {
        _Ty* tmp = (_Ty*)(::operator new(_Count));

        return tmp;
    }

    template <typename _Ty, typename _SizeType> inline
    _Ty *_Allocate(_Ty*, _SizeType _Count)
    {
        _Ty* tmp = (_Ty*)(::operator new(_Count * sizeof(_Ty)));

        return tmp;
    }

    template <typename _Ty, typename _SizeType> inline
    _Ty *_Allocate(_SizeType _Count)
    {
        _Ty* tmp = (_Ty*)(::operator new(_Count));

        return tmp;
    }

    template <typename _SizeType, typename _Ty> inline
    _Ty *_ReAllocate(_SizeType _Count, _Ty* _PtrOld)
    {
        _Ty* tmp = (_Ty*)(::operator new(_Count));
        memcpy(tmp, _PtrOld, _Count);
        if(_PtrOld != 0)
        {
            ::operator delete(_PtrOld);
        }

        return tmp;
    }

    template <typename _SizeType, typename _Ty> inline
        _Ty *_ReAllocate(_Ty* _PtrOld, _SizeType _Count)
    {
        _Ty* tmp = (_Ty*)(::operator new(_Count * sizeof(_Ty)));

        memcpy(tmp, _PtrOld, _Count * sizeof(_Ty));

        if (_PtrOld != 0)
        {
            ::operator delete(_PtrOld);
        }

        return tmp;
    }

    template <typename _Ty> inline
    void _Free(_Ty* _Ptr)
    {
        if(_Ptr != 0)
            ::operator delete(_Ptr);
    }

    template <class T1, class T2>
    inline void construct(T1* p, const T2& value)
    {
        new (p) T1(value);
    }

    template<class T>
    inline void destory(T* pointer)
    {
        pointer->~T();
    }
}

#endif