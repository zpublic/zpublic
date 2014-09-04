#pragma once
#include <windows.h>

template<bool t_bManaged>
class ZLModuleHandleT
{
public:
    ZLModuleHandleT( HMODULE hModule = NULL ) : m_hModule(hModule)
    {
    }

    ZLModuleHandleT( LPCTSTR pszPathFile )
    {
        if (pszPathFile != NULL)
            LoadLibrary(pszPathFile);
    }

    ~ZLModuleHandleT()
    {
        if (m_hModule != NULL && t_bManaged)
            FreeLibrary();
    }

public:
    HMODULE LoadLibrary(LPCTSTR pszPathFile)
    {
        m_hModule = ::LoadLibrary(pszPathFile);
        return m_hModule;
    }

    BOOL FreeLibrary()
    {
        BOOL bResult = FALSE;
        if (m_hModule != NULL)
            bResult = ::FreeLibrary(m_hModule);
        if (bResult)
            m_hModule = NULL;
        return bResult;
    }

    FARPROC GetProcAddress(LPCSTR pszProcName )
    {
        if (m_hModule == NULL)
            return NULL;
        return ::GetProcAddress(m_hModule, pszProcName);
    }

public:
    HMODULE Handle( void ) const     { return m_hModule   ; }
    void    Handle( HMODULE hModule) { m_hModule = hModule; };

protected:
    ZLModuleHandleT<t_bManaged>& operator = (const ZLModuleHandleT<t_bManaged>&);
    HMODULE m_hModule;
};

typedef ZLModuleHandleT<true>  ZLModule;
typedef ZLModuleHandleT<false> ZLModuleHandle;
