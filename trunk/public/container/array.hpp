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

#include "memory/variable_memory.hpp"
#include "tuple.hpp"

namespace zl
{
    typedef zl::Tuple<int, int>         DoublePos;
    typedef zl::Tuple<int, int, int>    ThreePos;

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

    template <typename T>
    class CArrayVariable
    {
    public:
        CArrayVariable(size_t Size = 0)
        {
            m_mem.Get(Size * sizeof(T));
            m_Size = Size;
        }
        ~CArrayVariable()
        {
            m_mem.Release();
        }

    public:
        T& operator[](size_t n)
        {
            return *(m_mem.Get() + n);
        }
        const T& operator[](size_t n) const
        {
            return *(m_mem.Get() + n);
        }
        size_t Size()
        {
            return m_Size;
        }
        void ReSize(size_t Size = 0)
        {
            m_mem.Get(Size * sizeof(T));
            m_Size = Size;
        }

    private:
        CVariableMem<T> m_mem;
        size_t m_Size;
    };

    template <typename T, size_t N>
    class CArrayFixedEx
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
    public:
        CArrayVariable<T>& operator[](DoublePos pos)
        {
            size_t iSize = pos.f1 - pos.f0;
            CArrayVariable<T> *pArr = new CArrayVariable<T>(iSize);
            for (size_t i = 0; i < iSize; ++i)
            {
                (*pArr)[i] = m_Arr[pos.f0 + i];
            }
            return *pArr;
        }
        const CArrayVariable<T>& operator[](DoublePos pos) const
        {
            size_t iSize = pos.f1 - pos.f0;
            CArrayVariable<T> *pArr = new CArrayVariable<T>(iSize);
            for (size_t i = 0; i < iSize; ++i)
            {
                (*pArr)[i] = m_Arr[pos.f0 + i];
            }
            return *pArr;
        }

        CArrayVariable<T>& operator[](ThreePos pos)
        {
            size_t iSize = (pos.f1 - pos.f0) / pos.f2;
            CArrayVariable<T> *pArr = new CArrayVariable<T>(iSize);
            for (size_t i = 0; i < iSize; ++i)
            {
                (*pArr)[i] = m_Arr[pos.f0 + i * pos.f2];
            }
            return *pArr;
        }
        const CArrayVariable<T>& operator[](ThreePos pos) const
        {
            size_t iSize = (pos.f1 - pos.f0) / pos.f2;
            CArrayVariable<T> *pArr = new CArrayVariable<T>(iSize);
            for (size_t i = 0; i < iSize; ++i)
            {
                (*pArr)[i] = m_Arr[pos.f0 + i * pos.f2];
            }
            return *pArr;
        }
    };
}

#endif