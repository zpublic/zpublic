#include "stdafx.h"
#include "test_basic.h"

//CComModule g_module;
CAppModule g_module;

int __stdcall WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    g_module.Init(0, hInstance);

    test_basic();

    return 0; // (int)msg.wParam;
}