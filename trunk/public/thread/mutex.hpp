/********************************************************************
* @file      : mutex.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/08 15:43
* @brief     : ª•≥‚ÃÂ
* 
* 
*********************************************************************/

#ifndef _MUTEX_
#define _MUTEX_

namespace zl
{

#include "basic/nocopyable.hpp"
#include "criticalsection.hpp"

    class CMutex : public NoCopyable
    {
    private:
        CriticalSection m_CriSection;

    public:
        CMutex()
        {
            m_CriSection.Init();
        }

        virtual ~CMutex()
        {
            m_CriSection.Del();
        }

        void Lock()
        {
            m_CriSection.Lock();
        }

        void Unlock()
        {
            m_CriSection.Unlock();
        }
    };

    class CMutexGuard : public NoCopyable
    {
    public:
        CMutexGuard(CMutex& mutex)
            : m_Mutex(mutex)
        {
            m_Mutex.Lock();
        }

        virtual ~CMutexGuard()
        {
            m_Mutex.Unlock();
        }

    private:
        CMutex& m_Mutex;
    };

}

#endif