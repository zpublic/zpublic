#include "stdafx.h"
#include "test_basic.h"
#include "test_gdi.h"
#include "test_gdiplus.h"
#include "../../pellets/z_win_utils/gdiplus.hpp"

//CComModule _Module;
CAppModule _Module;

int __stdcall WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    _Module.Init(0, hInstance);
    zl::WinUtils::ZLGdiPlusInit init;

    test_basic();
    test_gdi();
    test_gdiplus();

    return 0; // (int)msg.wParam;
}