// coeus.cpp : main source file for coeus.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include "game_login_dlg.h"

#include "resource.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    NET.Connect(L"127.0.0.1", 36911);
    GameLoginDlg dlg;
    return dlg.DoModal();
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{

    HRESULT hRes = ::CoInitialize(NULL);
    ATLASSERT(SUCCEEDED(hRes));

    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_BAR_CLASSES);

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    ///> ≥ı ºªØBKWIN
    BkString::Load(IDR_BK_STRING_DEF);
    BkSkin::LoadSkins(IDR_BK_SKIN_DEF);
    BkStyle::LoadStyles(IDR_BK_STYLE_DEF);

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    ::CoUninitialize();

    return nRet;
}
