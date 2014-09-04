#pragma once
#include "z_com_interface_ptr.hpp"

template <typename TInterface>
class ZLComLoader
{
public:
    TInterface* GetInterface()
    {
        return (TInterface*)m_mudule;
    }

protected:
    ZLComInterfacePtr<TInterface> m_mudule;

public:
    HRESULT Initialize(const wchar_t* pszDllPath = NULL)
    {
        HRESULT hr = S_OK;
        BOOL bRetVal = FALSE;
        if (pszDllPath)
            bRetVal = m_mudule.LoadInterface(pszDllPath);
        if (bRetVal == FALSE)
        {
            hr = GetLastError();
            return hr;
        }
        return hr;
    }

    HRESULT UnInitialize()
    {
        m_mudule.Free();
        return S_OK;
    }
};
