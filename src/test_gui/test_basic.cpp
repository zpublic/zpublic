#include "stdafx.h"
#include "dlg1.h"

void test_dlg1()
{
    CDlg1 dlg1;
    dlg1.Create(0, CWindow::rcDefault, L"dlg1 wnd", WS_OVERLAPPEDWINDOW, WS_EX_CLIENTEDGE);
    dlg1.CenterWindow();
    dlg1.ShowWindow(SW_SHOW);
    dlg1.UpdateWindow();

    MSG msg;
    msg.message = ~(UINT)WM_QUIT;
    while(msg.message != WM_QUIT)
    {
        if(::GetMessage(&msg, NULL, 0, 0))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }
}

void test_basic()
{
    test_dlg1();
}