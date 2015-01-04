#include "stdafx.h"
#include "ScriptGui.h"

int update_gui(lua_State* L)
{
	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();
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
