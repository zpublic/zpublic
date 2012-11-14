/********************************************************************
* @file      : increase_memory.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/14 17:00
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _INCREASE_MEMORY_
#define _INCREASE_MEMORY_

#include "memory/variable_memory.hpp"
#include "detail/increase_policy.hpp"

namespace zl
{

    template <typename T, typename IncreasePolicy = PIncreaseMul<2> >
    class CIncreaseMemory
    {
    public:
        CIncreaseMemory(unsigned long _Size = 0)
        {
            m_size = _Size;
            m_mem.Get(_Size);
        }
        bool Inc()
        {
            m_size = IncreasePolicy::Increase(m_size);
            return m_mem.Get(m_size) != INVALID_PTR;
        }
        T* Get()
        {
            return m_mem.Get();
        }
        void Release()
        {
            m_mem.Release();
            m_size = 0;
        }
        unsigned long Size()
        {
            return m_size;
        }

    private:
        CVariableMem<T> m_mem;
        unsigned long m_size;
    };

}

#endif