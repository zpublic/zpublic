#pragma once
#include "resource.h"

class CGdi4 : public CWindowImpl<CGdi4>
{
public:
    DECLARE_WND_CLASS(L"CGdi4")

    BEGIN_MSG_MAP(CGdi4)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_KEYDOWN(OnKeyDown)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
    END_MSG_MAP()

public:
    CGdi4()
    {
        m_hdcMem = NULL;
    }

    ~CGdi4()
    {
        if (m_hdcMem)
        {
            ::DeleteDC(m_hdcMem);
        }
    }

    void SetBg(HBITMAP hBitmap)
    {
        m_hBitmap = hBitmap;
    }

private:
    int OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        SetWindowLong(GWL_EXSTYLE, GetWindowLong(GWL_EXSTYLE) | WS_EX_LAYERED);
        COLORREF clTransparent = RGB(255, 255, 255);
        SetLayeredWindowAttributes(m_hWnd, clTransparent, 0, LWA_COLORKEY);

        HDC hdc = ::GetDC(m_hWnd);
        HDC hdcTemp = ::CreateCompatibleDC(hdc);
        ::SelectObject(hdcTemp, m_hBitmap);

        CRect rc;
        GetWindowRect(rc);
        m_hdcMem = ::CreateCompatibleDC(hdc);
        HBITMAP hbmp = ::CreateCompatibleBitmap(hdc, rc.Width(), rc.Height());  
        ::SelectObject(m_hdcMem, hbmp);

        BITMAP bm;
        ::GetObject(m_hBitmap, sizeof(bm), &bm);
        ::StretchBlt(m_hdcMem, 0, 0, rc.Width(), rc.Height(), hdcTemp, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

        ::DeleteDC(hdcTemp);
        ::ReleaseDC(m_hWnd, hdc);
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

    BOOL OnEraseBkgnd(CDCHandle dc)
    {
        CRect rc;
        GetWindowRect(rc);
        ::BitBlt(dc, 0, 0, rc.Width(), rc.Height(), m_hdcMem, 0, 0, SRCCOPY);
        return TRUE;
    }

private:
    HDC m_hdcMem;
    HBITMAP m_hBitmap;
};

void gdi4()
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

    CGdi4 dlg;
    CRect rect(0, 0, bm.bmWidth * 2, bm.bmHeight * 2);
    dlg.SetBg(hBitmap);
    dlg.Create(0, rect, L"CGdi4 wnd", WS_POPUP, WS_EX_TOPMOST);
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
