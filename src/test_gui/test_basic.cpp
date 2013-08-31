#include "stdafx.h"
#include "dlg1.h"
#include "dlg2.h"
#include "dlg3.h"
#include "dlg4.h"
#include "frame1.h"
#include "frame2.h"
#include "frame3.h"

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

void test_frame1()
{
    CFrame1 frame1;
    MSG msg;

    assert(frame1.CreateEx());

    frame1.CenterWindow();
    frame1.ShowWindow (SW_SHOW);
    frame1.UpdateWindow();

    while (::GetMessage (&msg, NULL, 0, 0 ) > 0)
    {
        ::TranslateMessage (&msg);
        ::DispatchMessage (&msg);
    }
}

void test_frame2()
{
    CFrame2 frame2;
    MSG msg;

    assert(frame2.CreateEx());

    frame2.CenterWindow();
    frame2.ShowWindow (SW_SHOW);
    frame2.UpdateWindow();

    while (::GetMessage (&msg, NULL, 0, 0 ) > 0)
    {
        ::TranslateMessage (&msg);
        ::DispatchMessage (&msg);
    }
}

void test_frame3()
{
    CFrame3 frame3;
    MSG msg;

    assert(frame3.CreateEx());

    frame3.CenterWindow();
    frame3.ShowWindow (SW_SHOW);
    frame3.UpdateWindow();

    while (::GetMessage (&msg, NULL, 0, 0 ) > 0)
    {
        ::TranslateMessage (&msg);
        ::DispatchMessage (&msg);
    }
}

void test_basic()
{
    //test_dlg1();
    //test_dlg2();
    //test_dlg3();
    //test_dlg4();
    //test_frame1();
    //test_frame2();
    //test_frame3();
}