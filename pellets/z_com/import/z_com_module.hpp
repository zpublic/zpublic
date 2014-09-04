#pragma once
#include "z_module_handle.hpp"

#define ZL_PFN_DLLGETCLASSOBJECT  ("DllGetClassObject")
#define ZL_PFN_DLLCANUNLOADNOW    ("DllCanUnloadNow"  )

typedef HRESULT STDAPICALLTYPE pfnZLDllCanUnloadNow(void);
typedef HRESULT STDAPICALLTYPE pfnZLDllGetClassObject(
    REFCLSID rclsid, 
    REFIID riid, 
    LPVOID* ppv);

template<bool t_bManaged>
class ZLComModuleT : public ZLModuleHandleT<t_bManaged>
{
public:
    ZLComModuleT( HMODULE hModule = NULL ) :
        m_pfnDllCanUnloadNow(NULL),
        m_pfnDllGetClassObject(NULL),
        ZLModuleHandleT<t_bManaged>(hModule)
    {
        _ComInitFuncs();
    }

    ZLComModuleT( LPCTSTR pszPathFile ) :
        m_pfnDllCanUnloadNow(NULL),
        m_pfnDllGetClassObject(NULL)
    {
        if (pszPathFile != NULL)
            LoadLibrary(pszPathFile);
    }

public:
    HMODULE LoadLibrary(LPCTSTR pszPathFile)
    {
        m_hModule = ZLModuleHandleT<t_bManaged>::LoadLibrary(pszPathFile);
        HRESULT hRet = _ComInitFuncs();
        return m_hModule;
    }

    HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
    {
        if (m_pfnDllGetClassObject == NULL || m_hModule == NULL)
            return E_FAIL;
        return m_pfnDllGetClassObject(rclsid, riid, ppv);
    }

    HRESULT DllCanUnloadNow()
    {
        if (m_pfnDllCanUnloadNow == NULL || m_hModule == NULL)
            return S_OK;
        return m_pfnDllCanUnloadNow();
    }

protected:
    HRESULT _ComInitFuncs()
    {
        HRESULT hResult = E_FAIL;
        if (Handle() == NULL)
            return E_FAIL;

        m_pfnDllCanUnloadNow = 
            (pfnZLDllCanUnloadNow*)GetProcAddress(ZL_PFN_DLLCANUNLOADNOW);
        m_pfnDllGetClassObject = 
            (pfnZLDllGetClassObject*)GetProcAddress(ZL_PFN_DLLGETCLASSOBJECT);

        if (m_pfnDllCanUnloadNow != NULL || m_pfnDllGetClassObject != NULL)
            hResult = S_OK;
        return hResult;
    }

private:
    pfnZLDllCanUnloadNow*   m_pfnDllCanUnloadNow;
    pfnZLDllGetClassObject* m_pfnDllGetClassObject;
};

typedef ZLComModuleT<true>  ZLComModule;
typedef ZLComModuleT<false> ZLComModuleHandle;
