#pragma once
#include <set>
#include "observer.h"

class Observable
{
public:
    Observable() : m_bChanged(false)
    {

    }

    virtual ~Observable()
    {

    }

    virtual bool addObserver(Observer* pObserver)
    {
        std::pair<std::set<Observer*>::iterator, bool> ret = m_Observers.insert(pObserver);
        return ret.second;
    }

    virtual bool deleteObserver(Observer* pObserver)
    {
        std::set<Observer*>::iterator it = m_Observers.find(pObserver);
        if (it != m_Observers.end())
        {
            m_Observers.erase(it);
            return true;
        }
        return false;
    }

    virtual void notifyObservers(void* lpData = 0)
    {
        if (m_bChanged)
        {
            std::set<Observer*>::iterator it = m_Observers.begin();
            std::set<Observer*>::iterator itEnd = m_Observers.end();
            for (; it != itEnd; ++it)
            {
                (*it)->update(this, lpData);
            }
            m_bChanged = false;
        }
    }

    virtual void setChanged()
    {
        m_bChanged = true;
    }

protected:
    std::set<Observer*> m_Observers;
    bool m_bChanged;
};