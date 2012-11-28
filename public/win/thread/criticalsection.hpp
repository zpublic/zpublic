/********************************************************************
* @file      : criticalsection.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/08 15:30
* @brief     : ÁÙ½çÇø
* 
* 
*********************************************************************/

#ifndef _CRITICAL_SECTION_
#define _CRITICAL_SECTION_

#include <windows.h>

namespace zl
{

    class CriticalSection
    {
    private:
        CRITICAL_SECTION m_lock;

    public:
        void Init()
        {
            ::InitializeCriticalSection(&m_lock);
        }
        void Del()
        {
            ::DeleteCriticalSection(&m_lock);
        }
        void Lock()
        {
            ::EnterCriticalSection(&m_lock);
        }
        void Unlock()
        {
            ::LeaveCriticalSection(&m_lock);
        }
    };

}

#endif