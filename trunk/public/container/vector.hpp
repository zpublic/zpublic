/********************************************************************
* @file      : vector.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/19 16:16
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include "../detail/increase_policy.hpp"
#include "../memory/variable_memory.hpp"

#define DEFAULT_VECTOR_SIZE 4

namespace zl
{

    template <typename T>
    class CSimpleVector
    {
    private:
        CVariableMem<T> m_mem;
        int m_Size;
        int m_SizeAlloc;

    public:
        CSimpleVector(int SizeAlloc = DEFAULT_VECTOR_SIZE)
            : m_Size(0)
            , m_SizeAlloc(SizeAlloc)
        {
            m_mem.Get(m_SizeAlloc * sizeof(T));
        }

        CSimpleVector(const CSimpleVector<T>& src)
            : m_Size(0)
            , m_SizeAlloc(DEFAULT_VECTOR_SIZE)
        {
            if (src.GetSize())
            {
                m_Size = src.GetSize();
                m_SizeAlloc = src.GetAllocSize();
                ::memcpy(
                    m_mem.Get(src.GetAllocSize() * sizeof(T)),
                    src.GetData(),
                    src.GetSize() * sizeof(T));
            }
            else
            {
                m_mem.Get(m_SizeAlloc * sizeof(T));
            }
        }

        CSimpleVector<T>& operator=(const CSimpleVector<T>& src)
        {
            if (GetAllocSize() != src.GetAllocSize())
            {
                RemoveAll();
                m_Size = src.GetSize();
                m_SizeAlloc = src.GetAllocSize();
                ::memcpy(
                    m_mem.Get(src.GetAllocSize() * sizeof(T)),
                    src.GetData(),
                    src.GetSize() * sizeof(T));
            }
            else
            {
                if (GetSize() != src.GetSize())
                {
                    m_Size = src.GetSize();
                }
                ::memcpy(
                    m_mem.Get(),
                    src.GetData(),
                    src.GetSize() * sizeof(T));
            }
            return (*this);
        }

        ~CSimpleVector()
        {
            RemoveAll();
        }

        const T& operator[] (int nIndex) const
        {
            return m_mem.Get()[nIndex];
        }
        T& operator[] (_In_ int nIndex)
        {
            return m_mem.Get()[nIndex];
        }

    public:
        int GetSize() const
        {
            return m_Size;
        }

        int GetAllocSize() const
        {
            return m_SizeAlloc;
        }

        void RemoveAll()
        {
            m_mem.Release();
            m_Size = 0;
            m_SizeAlloc = 0;
        }

        void RemoveData()
        {
            m_Size = 0;
        }

        T* GetData() const
        {
            return (T*)m_mem.Get();
        }

        bool Add(const T& t)
        {
            if(m_Size == m_SizeAlloc)
            {
                m_SizeAlloc = DoubleIncrese::Increase(m_SizeAlloc);
                if(m_mem.Get(m_SizeAlloc * sizeof(T)) == NULL)
                    return false;
            }
            m_mem.Get()[m_Size] = t;
            m_Size++;
            return true;
        }

        bool RemoveAt(int nIndex)
        {
            if (nIndex < 0 || nIndex >= m_Size)
                return false;
            if (nIndex != m_Size - 1 && m_Size != 1)
            {
                ::memcpy(
                    &(m_mem.Get()[nIndex]),
                    &(m_mem.Get()[nIndex + 1]),
                    (m_Size - nIndex) * sizeof(T));
            }
            m_Size--;
            return true;
        }
    };

}

#endif