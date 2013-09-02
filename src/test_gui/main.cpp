#include "stdafx.h"
#include "test_basic.h"
#include "test_gdi.h"
#include "test/test_zlui.h"
#include "test_lab.h"

//CComModule _Module;
CAppModule _Module;

int __stdcall WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    _Module.Init(0, hInstance);

    test_basic();
    test_gdi();
    test_zlui();
    test_lab();

    return 0; // (int)msg.wParam;
}