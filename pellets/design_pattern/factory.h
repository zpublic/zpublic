#pragma once
#include <map>

template <typename T, typename T2>
T* CreateObject()
{
    T2* p = new T2;
    return static_cast<T*>(p);
}

template <typename T>
class SimpleFactory
{
public:
    typedef T* (*fnCreateObject)(void);

    T* CreateObject(int id)
    {
        std::map<int, fnCreateObject>::iterator it = m_factory.find(id);
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

    void AddCreator(int id, fnCreateObject pFunc)
    {
        m_factory[id] = pFunc;
    }
    void DeleteCreator(int id)
    {
       std::map<int, fnCreateObject>::iterator it = m_factory.find(id);
       if (it != m_factory.end())
       {
           m_factory.erase(it);
       }
    }

private:
    std::map<int, fnCreateObject> m_factory;
};
