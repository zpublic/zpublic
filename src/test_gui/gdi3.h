#pragma once
#include "resource.h"

class CGdi3 : public CWindowImpl<CGdi3>
{
public:
    DECLARE_WND_CLASS(L"CGdi3")

    BEGIN_MSG_MAP(CGdi3)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_KEYDOWN(OnKeyDown)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
    END_MSG_MAP()

private:
    int OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        SetWindowLong(GWL_EXSTYLE, GetWindowLong(GWL_EXSTYLE) | WS_EX_LAYERED);
        COLORREF clTransparent = RGB(255, 255, 255);
        ::SetLayeredWindowAttributes(m_hWnd, clTransparent, 0, LWA_COLORKEY);
        return 0;
    }

    void OnFinalMessage(HWND hWnd)
    {
        ::PostQuitMessage(0);
    }

    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        if (VK_ESCAPE == nChar)
        {
            DestroyWindow();
        }
        else
        {
            SetMsgHandled(FALSE);
        }
    }

    void OnLButtonDown(UINT nFlags, CPoint point)
    {
        PostMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
        SetMsgHandled(FALSE);
    }
};

void gdi3()
{
    HBITMAP hBitmap = (HBITMAP)LoadImage(
        _Module.m_hInst,
        MAKEINTRESOURCE(IDB_BITMAP2),
        IMAGE_BITMAP,
        0,
        0,
        LR_CREATEDIBSECTION);
    BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);

    CGdi3 dlg;
    CWndClassInfo& wc = dlg.GetWndClassInfo();
    wc.m_wc.hbrBackground = CreatePatternBrush(hBitmap);
    CRect rect(0, 0, bm.bmWidth, bm.bmHeight);
    dlg.Create(0, rect, L"CGdi3 wnd", WS_POPUP, WS_EX_TOPMOST);
    dlg.CenterWindow();
    dlg.ShowWindow(SW_SHOW);
    dlg.UpdateWindow();

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
