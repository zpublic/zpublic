#pragma once
#include <windows.h>
#include <unknwn.h>
#include <atlcomcli.h>

namespace _internal
{
    template <class Type>
    class _NoAddRefReleaseOnCComPtr : public Type
    {
    private:
        STDMETHOD_(ULONG, AddRef)() = 0;
        STDMETHOD_(ULONG, Release)() = 0;
    };
}

template<class T>
class ZLComAutoPtr
{
    typedef T _PtrClass;

public:
    ZLComAutoPtr() throw() : m_pT(NULL)
    {
    }
    ZLComAutoPtr(int nNull) throw() : m_pT(NULL)
    {
    }
    ZLComAutoPtr(T* lp) throw()
    {
        m_pT = lp;
        if (m_pT != NULL)
            m_pT->AddRef();
    }
    ~ZLComAutoPtr() throw()
    {
        if (m_pT)
            m_pT->Release();
    }

    operator T*() const throw()
    {
        return m_pT;
    }
    T& operator*() const
    {
        return *m_pT;
    }
    T** operator&() throw()
    {
        return &m_pT;
    }

    _internal::_NoAddRefReleaseOnCComPtr<T>* operator->() const throw()
    {
        return (_internal::_NoAddRefReleaseOnCComPtr<T>*)m_pT;
    }
    bool operator!() const throw()
    {
        return (m_pT == NULL);
    }
    bool operator<(T* pT) const throw()
    {
        return m_pT < pT;
    }
    bool operator!=(T* pT) const
    {
        return !operator==(pT);
    }
    bool operator==(T* pT) const throw()
    {
        return m_pT == pT;
    }

    void Release() throw()
    {
        T* pTemp = m_pT;
        if (pTemp)
        {
            m_pT = NULL;
            pTemp->Release();
        }
    }

    bool IsEqualObject(IUnknown* pOther) throw()
    {
        if (m_pT == NULL && pOther == NULL)
            return true;
        if (m_pT == NULL || pOther == NULL)
            return false;

        CComPtr<IUnknown> punk1;
        CComPtr<IUnknown> punk2;
        m_pT->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
        pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
        return punk1 == punk2;
    }

    void Attach( T* p2) throw()
    {
        if (m_pT)
            m_pT->Release();
        m_pT = p2;
    }

    T* Detach() throw()
    {
        T* pt = m_pT;
        m_pT = NULL;
        return pt;
    }

    HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw()
    {
        return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&m_pT);
    }

    HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw()
    {
        CLSID clsid;
        HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
        if (SUCCEEDED(hr))
            hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&m_pT);
        return hr;
    }

    template <class Q>
    HRESULT QueryInterface(Q** pp) const throw()
    {
        if (pp == NULL || m_pT == NULL)
            return E_FAIL;
        return m_pT->QueryInterface(__uuidof(Q), (void**)pp);
    }

public:
    T* m_pT;
};
