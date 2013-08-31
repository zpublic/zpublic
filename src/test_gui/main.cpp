#include "stdafx.h"
#include "test_basic.h"
#include "test_gdi.h"
#include "test/test_zlui.h"

//CComModule g_module;
CAppModule g_module;

int __stdcall WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    g_module.Init(0, hInstance);

    test_basic();
    test_gdi();
    test_zlui();

    return 0; // (int)msg.wParam;
}