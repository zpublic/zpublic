/********************************************************************
* @file      : variable_memory.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/14 16:31
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _VARIABLE_MEMORY_
#define _VARIABLE_MEMORY_

#include "memory/allocer.hpp"

namespace zl
{

#ifndef INVALID_PTR
#define INVALID_PTR 0
#endif

    template <typename T>
    class CVariableMem
    {
    public:
        CVariableMem() : m_p(INVALID_PTR) {}
        ~CVariableMem() {}

    public:
        T* Get()
        {
            return m_p;
        }
        const T* Get() const
        {
            return m_p;
        }
        T* Get(unsigned long _Size)
        {
            if (m_p == INVALID_PTR)
            {
                m_p = _Allocate(_Size, m_p);
            }
            else
            {
                m_p = _ReAllocate(_Size, m_p);
            }
            return m_p;
        }
        void Release()
        {
            if (m_p != INVALID_PTR)
            {
                _Free(m_p);
                m_p = INVALID_PTR;
            }
        }

    private:
        T* m_p;
    };

}

#endif