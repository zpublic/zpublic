#include "stdafx.h"
#include "dlg1.h"
#include "dlg2.h"
#include "dlg3.h"
#include "dlg4.h"

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
    assert(msg.lParam == 0);
}

void test_dlg2()
{
    CDlg2* pdlg2 = new CDlg2;
    pdlg2->Create(0);
    pdlg2->ShowWindow(SW_SHOW);

    CDlg2 dlg2;
    assert(IDOK == dlg2.DoModal());
}

void test_dlg3()
{
    CDlg3 dlg3;
    assert(IDCANCEL == dlg3.DoModal());
}

void test_dlg4()
{
    CDlg4 dlg4;
    assert(IDOK == dlg4.DoModal());
}

void test_basic()
{
    //test_dlg1();
    //test_dlg2();
    //test_dlg3();
    test_dlg4();
}