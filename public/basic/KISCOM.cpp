/********************************************************************
//	File Name	:	KISCOM.cpp
//	Version		:	1.0
//	Datetime	:	2006-3-23	0:37	--	Created.
//	Author		:	Chen Rui
//	Comment		:	
*********************************************************************/

#include "stdafx.h"

#include "KISCOM.h"

typedef HRESULT (STDAPICALLTYPE* PFN_DLL_GET_CLASS_OBJECT)(
    /* [in ] */ REFCLSID rclsid, 
    /* [in ] */ REFIID riid, 
    /* [out, iid_is(riid)] */ LPVOID* ppv);
typedef HRESULT (STDAPICALLTYPE* PFN_DLL_CAN_UNLOAD_NOW)(void);


STDAPI_(HMODULE) KISCoLoadLibraryA(
    /* [in, string, max_is(MAX_PATH)] */ const char szFileNameA[])
{
    return (szFileNameA ? ::LoadLibraryA(szFileNameA) : NULL);
}

STDAPI_(HMODULE) KISCoLoadLibrary(
    /* [in, string, max_is(MAX_PATH)] */ const TCHAR szFileName[])
{
    return (szFileName ? ::LoadLibrary(szFileName) : NULL);
}

STDAPI KISCoFreeLibrary(
    /* [in ] */ HMODULE hModule)
{
    if (!hModule)
        return E_INVALIDARG;
    return (::FreeLibrary(hModule) ? S_OK : E_FAIL);
}

STDAPI KISCoGetClassObject(
    /* [in ] */ HMODULE hModule,
    /* [in ] */ REFIID riid, 
    /* [out, iid_is(riid)] */ LPVOID* ppv)
{
    PFN_DLL_GET_CLASS_OBJECT pfnDllGetClassObject;
    CLSID clsid;

    if (!ppv)
        return E_POINTER;
    if (!hModule)
        return E_INVALIDARG;

    pfnDllGetClassObject 
        = (PFN_DLL_GET_CLASS_OBJECT)GetProcAddress(hModule, "DllGetClassObject");
        
    return (pfnDllGetClassObject ? pfnDllGetClassObject(clsid, riid, ppv) : CO_E_ERRORINDLL);
}

STDAPI KISCoCreateInstance(
    /* [in ] */ HMODULE hModule,
    /* [in ] */ REFIID riid, 
    /* [out, iid_is(riid)] */ LPVOID* ppv)
{
    HRESULT hr;
    IClassFactory* piFactory;

    hr = KISCoGetClassObject(hModule, IID_IClassFactory, (LPVOID*)&piFactory);
    if (SUCCEEDED(hr))
    {
        hr = piFactory->CreateInstance(NULL, riid, ppv);
        piFactory->Release();
    }
    return hr;
}
