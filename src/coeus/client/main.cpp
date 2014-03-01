// coeus.cpp : main source file for coeus.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include "window_manager.h"

#include "resource.h"

#pragma comment(lib, "../lib/venus.lib")

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    NET.Connect("127.0.0.1", 36911);

    int hasError=0;
    UINT_PTR current_main_window = STARTUP_WINDOW;
    UINT_PTR nRet;
    while(nRet = GameWindowControl::start(current_main_window)){
        if(nRet == GWIN_ID_SUCCESS_EXIT){
            break;
        }
        if(nRet == GWIN_ID_EXIT){
            hasError=1;
            break;
        }
        current_main_window = nRet;
    }
    
    // x@x.x
    NET.Close();
    return hasError;
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
