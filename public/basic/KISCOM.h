/********************************************************************
//	FileName    :   KISCOM.h
//	Version     :   2.0
//	Author      :	Chen Rui
//	Date        :   2005-05-06  1:48  --  Created.
//                  2006-02-20  11:11 --  Modified, upgrade to 1.1.
//                  2006-05-07  03:46 --  Modified, upgrade to 2.0.
//	Comment     :	A mini-COM for KIS.
*********************************************************************/

#pragma once

#include <unknwn.h>

template < class TInterface > 
class CKISComObject :   public TInterface
{
public:
    // implement abstract methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in ] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR* __RPC_FAR* ppvObject)
    {
        if (!ppvObject)
            return E_POINTER;
        if ( (IID_IUnknown         == riid) ||
             (__uuidof(TInterface) == riid) )
        {
            *ppvObject = static_cast< IUnknown* >(this);
        }
        else                                     
        {
            *ppvObject = NULL;
            return E_NOINTERFACE;
        }
        (static_cast< IUnknown* >(*ppvObject))->AddRef();
        return S_OK;
    }
    virtual ULONG STDMETHODCALLTYPE AddRef(void)
    {
        return InterlockedIncrement((LONG volatile *)&m_cRef);
    }
    virtual ULONG STDMETHODCALLTYPE Release(void)
    {
        if (0L == InterlockedDecrement((LONG volatile *)&m_cRef))
        {
            delete this;
            return 0L;
        }
        return m_cRef;
    }
    // constructor
    CKISComObject() : m_cRef(1L)
    {
    }
private:
    LONG m_cRef;
};

template < class TInterface1, class TInterface2 > 
class CKISComObject2    :   public TInterface1, public TInterface2
{
public:
    // implement abstract methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in ] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR* __RPC_FAR* ppvObject)
    {
        if (!ppvObject)
            return E_POINTER;
        if ( (IID_IUnknown           == riid)  ||
             (__uuidof(TInterface1)  == riid) 
             (__uuidof(TInterface2)  == riid) )
        {
            *ppvObject = static_cast< IUnknown* >(this);
        }
        else                                     
        {
            *ppvObject = NULL;
            return E_NOINTERFACE;
        }
        (static_cast< IUnknown* >(*ppvObject))->AddRef();
        return S_OK;
    }
    virtual ULONG STDMETHODCALLTYPE AddRef(void)
    {
        return InterlockedIncrement((LONG volatile *)&m_cRef);
    }
    virtual ULONG STDMETHODCALLTYPE Release(void)
    {
        if (0L == InterlockedDecrement((LONG volatile *)&m_cRef))
        {
            delete this;
            return 0L;
        }
        return m_cRef;
    }
    // constructor
    CKISComObject2() : m_cRef(1L)
    {
    }
private:
    LONG m_cRef;
};

template < class TClass, bool fSingleton = false>
class CKISClassFactory  :   public CKISComObject< IClassFactory >
{
public:
    // implement abstract methods
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstance( 
        /* [unique][in] */ IUnknown *pUnkOuter,
        /* [in ] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject)
    {
        HRESULT hr;
        TClass* p;
        if (!ppvObject)
            return E_POINTER;
        if (pUnkOuter)
            return CLASS_E_NOAGGREGATION;
        if (fSingleton && (m_cInst > 0L))
            return CLASS_E_CLASSNOTAVAILABLE;
        p = new TClass;
        if (!p)
            return E_OUTOFMEMORY;
        hr = p->QueryInterface(riid, ppvObject);
        p->Release();
        if (SUCCEEDED(hr))
            InterlockedIncrement((LONG volatile *)&m_cInst);
        return hr;
    }
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE LockServer( 
        /* [in ] */ BOOL fLock)
    {
        return E_NOTIMPL;
    }
    // constructor
    CKISClassFactory() : m_cInst(0L)
    {
    }
private:
    LONG m_cInst;
};

[
    uuid("B85B177A-4250-4f12-9236-DE10CCF69A1B")
]
interface IKISComModule :   IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetClassObject(
        /* [in ] */ REFIID riid, 
        /* [out, iid_is(riid)] */ LPVOID* ppv) = 0;
    virtual HRESULT STDMETHODCALLTYPE CanUnloadNow(void) = 0;
};

template < class TClass, bool fSingleton>
HRESULT GetClassFactory(
    /* [in ] */ const IID& iid, 
    /* [out, iid_is(riid)] */ IClassFactory** ppiFactory)
{
    typedef CKISClassFactory< TClass, fSingleton >  _TClassFactory;
    HRESULT hr;
    _TClassFactory* p;
    if (!ppiFactory)
        return E_POINTER;
    if ( (IID_IUnknown      != iid) &&
         (IID_IClassFactory != iid) )
        return E_NOINTERFACE;
    p = new _TClassFactory;
    if (!p)
        return E_OUTOFMEMORY;
    hr = p->QueryInterface(IID_IClassFactory, (LPVOID*)ppiFactory);
    p->Release();
    return hr;
}

STDAPI_(HMODULE) KISCoLoadLibraryA(
    /* [in, string, max_is(MAX_PATH)] */ const char szFileNameA[]);
STDAPI_(HMODULE) KISCoLoadLibrary(
    /* [in, string, max_is(MAX_PATH)] */ const TCHAR szFileName[]);
STDAPI KISCoFreeLibrary(
    /* [in ] */ HMODULE hModule);
STDAPI KISCoGetClassObject(
    /* [in ] */ HMODULE hModule,
    /* [in ] */ REFIID riid, 
    /* [out, iid_is(riid)] */ LPVOID* ppv);
STDAPI KISCoCreateInstance(
    /* [in ] */ HMODULE hModule,
    /* [in ] */ REFIID riid, 
    /* [out, iid_is(riid)] */ LPVOID* ppv);

class CKISUseComDll
{
public:
    HRESULT STDMETHODCALLTYPE LoadA(
        /* [in, string, max_is(MAX_PATH)] */ const char szFileNameA[])
    {
        if (m_hModule)
            return E_FAIL;
        m_hModule = ::KISCoLoadLibraryA(szFileNameA);
        return (m_hModule ? S_OK : E_FAIL);
    }
    HRESULT STDMETHODCALLTYPE Load(
        /* [in, string, max_is(MAX_PATH)] */ const TCHAR szFileName[])
    {
        if (m_hModule)
            return E_FAIL;
        m_hModule = ::KISCoLoadLibrary(szFileName);
        return (m_hModule ? S_OK : E_FAIL);
    }
    HRESULT STDMETHODCALLTYPE Free(void)
    {
        HRESULT hr;
        hr = ::KISCoFreeLibrary(m_hModule);
        if (SUCCEEDED(hr))
            m_hModule = NULL;
        return hr;
    }
    HRESULT STDMETHODCALLTYPE GetClassObject(
        /* [in ] */ REFIID riid, 
        /* [out, iid_is(riid)] */ LPVOID* ppv)
    {
        return ::KISCoGetClassObject(m_hModule, riid, ppv);
    }
    HRESULT STDMETHODCALLTYPE CreateInstance(
        /* [in ] */ REFIID riid, 
        /* [out, iid_is(riid)] */ LPVOID* ppv)
    {
        return ::KISCoCreateInstance(m_hModule, riid, ppv);
    }
    // constructor
    CKISUseComDll() : m_hModule(NULL)
    {
    }
    //CKISUseComDll(
    //    /* [in, string, max_is(MAX_PATH)] */ const char szFileNameA[]) : m_hModule(NULL)
    //{
    //    LoadA(szFileNameA);
    //}
    CKISUseComDll(
        /* [in, string, max_is(MAX_PATH)] */ const TCHAR szFileName[]) : m_hModule(NULL)
    {
        Load(szFileName);
    }
    virtual ~CKISUseComDll()
    {
        if (m_hModule)
            Free();
    }
private:
    HMODULE m_hModule;
};
