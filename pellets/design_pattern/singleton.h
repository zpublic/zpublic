#pragma once

template<typename T>
class Singleton
{
public:
    static T* getInstance()
    {
        if (m_pInst == NULL)
        {
            m_pInst = new T();
        }
        return m_pInst;
    }

    static T& Instance()
    {
        return *(getInstance());
    }

    operator T*() const throw()
    {
        return m_pInst ;
    }

    T* operator->() const throw()
    {
        return m_pInst ;
    }

    static void Destroy()
    {
        if (m_pInst != NULL)
        {
            delete m_pInst;
        }
        m_pInst = NULL;
    }

    virtual bool Initialize() { return true; }

private:
    Singleton() {}
    Singleton(const Singleton &s);
    Singleton& operator=(const Singleton &s);
    static T* m_pInst;

};

template< typename T >
T* Singleton<T>::m_pInst;
