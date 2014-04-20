#pragma once
#include <map>

template <typename T, typename T2>
T* CreateObject()
{
    T2* p = new T2;
    return static_cast<T*>(p);
}

template <typename T, typename TKey = int>
class SimpleFactory
{
public:
    typedef T* (*fnCreateObject)(void);

    T* CreateObject(TKey id)
    {
        std::map<TKey, fnCreateObject>::iterator it = m_factory.find(id);
        if (it != m_factory.end())
        {
            fnCreateObject pFunc = it->second;
            if (pFunc)
            {
                return pFunc();
            }
        }
        return 0;
    }

    void AddCreator(TKey id, fnCreateObject pFunc)
    {
        m_factory[id] = pFunc;
    }
    void DeleteCreator(TKey id)
    {
       std::map<TKey, fnCreateObject>::iterator it = m_factory.find(id);
       if (it != m_factory.end())
       {
           m_factory.erase(it);
       }
    }

private:
    std::map<TKey, fnCreateObject> m_factory;
};
