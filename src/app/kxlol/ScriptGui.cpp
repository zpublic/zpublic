#include "stdafx.h"
#include "ScriptGui.h"
#include "kxlol.h"

extern CkxlolApp theApp;

int update_gui(lua_State* L)
{
    HWND hWnd = theApp.m_pMainWnd->m_hWnd;
	::PostMessage(hWnd, KXLOL_WM_UPDATEGUI, 0, 0);
    return 0;
}

int messagebox(lua_State* L)
{
    const char* lpMsg = lua_tostring(L, 1);
    const char* lpTitle = lua_tostring(L, 2);
    if (lpMsg && lpTitle)
    {
        ::MessageBox(0, ZLA2W(lpMsg, CP_UTF8), ZLA2W(lpTitle, CP_UTF8), 0);
    }
    return 0;
}

int info_output(lua_State* L)
{
    const char* lpMsg = lua_tostring(L, 1);
    if (lpMsg)
    {
        HWND hWnd = theApp.m_pMainWnd->m_hWnd;
        ::SendMessage(hWnd, KXLOL_WM_INFO_OUTPUT, (WPARAM)ZLA2W(lpMsg, CP_UTF8).GetString(), 0);
    }
    return 0;
}
