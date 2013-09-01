#pragma once
#include "resource.h"
#include "animate.h"

const int WM_DO_ANIMATE = WM_USER + 1;

class CGdi9Dlg : public CSimpleDialog<IDD_DIALOG_GDI1>
{
public:
    BEGIN_MSG_MAP(CGdi9Dlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_PAINT(OnPaint)
        MESSAGE_HANDLER(WM_DO_ANIMATE, OnAnimate)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

public:
    CGdi9Dlg()
    {
        m_nAniType = 1;
    }
    ~CGdi9Dlg()
    {
        m_hdcMem.DeleteDC();
    }

private:
    BOOL OnInitDialog(CWindow wnd, LPARAM)
    {
        HBITMAP hBitmap = (HBITMAP)LoadImage(
            g_module.m_hInst,
            MAKEINTRESOURCE(IDB_BITMAP1),
            IMAGE_BITMAP,
            0,
            0,
            LR_CREATEDIBSECTION);
        if (hBitmap)  
        {  
            CDCHandle dc = wnd.GetDC();
            m_hdcMem.CreateCompatibleDC(dc);
            m_hdcMem.SelectBitmap(hBitmap);
            wnd,ReleaseDC(dc);
            ::GetObject(hBitmap, sizeof(m_bm), &m_bm); 
        }

        MoveWindow(0, 0, m_bm.bmWidth, m_bm.bmHeight);
        CenterWindow();
        return TRUE;
    }

    LRESULT OnAnimate(UINT, WPARAM, LPARAM, BOOL& bHandled)
    {
        bHandled = TRUE;
        m_nAniType++;
        if (m_nAniType > 13)
        {
            m_nAniType = 0;
        }
        else
        {
            InvalidateRect(NULL, TRUE);
        }
        return 0;
    }

    void OnPaint(CDCHandle dc)
    {
        HDC             hdc;
        PAINTSTRUCT     ps;
        hdc = BeginPaint(&ps);
        switch (m_nAniType)
        {
        case 0:
            BitBlt(hdc, 0, 0, m_bm.bmWidth, m_bm.bmHeight, m_hdcMem, 0, 0, SRCCOPY);
            break;
        case 1:
            AnimateDraw_StaggeredHorizontal(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight, 3);
            break;
        case 2:
            AnimateDraw_StaggeredVertical(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight, 3);
            break;
        case 3:
            AnimateDraw_JalousieHorizontal(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight, 10);
            break;
        case 4:
            AnimateDraw_JalousieVertical(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight, 10);
            break;
        case 5:
            AnimateDraw_RandomBlocks(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight, 12);
            break;
        case 6:
            AnimateDraw_FlyingTopToBottom(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight, 2, FALSE);
            break;
        case 7:
            AnimateDraw_FlyingBottomToTop(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight, 2, FALSE);
            break;	
        case 8:
            AnimateDraw_FlyingLeftToRight(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight, 2, FALSE);
            break;
        case 9:
            AnimateDraw_FlyingRightToLeft(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight, 2, FALSE);
            break;
        case 10:
            AnimateDraw_FlyingTopToBottom(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight);
            break;
        case 11:
            AnimateDraw_FlyingBottomToTop(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight);
            break;	
        case 12:
            AnimateDraw_FlyingLeftToRight(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight);
            break;
        case 13:
            AnimateDraw_FlyingRightToLeft(hdc, m_hdcMem, m_bm.bmWidth, m_bm.bmHeight);
            break;
        }
        if (m_nAniType != 0)
        {
            InvalidateRect(NULL, FALSE);
            PostMessage(WM_DO_ANIMATE);
        }
        EndPaint(&ps);
    }

    LRESULT OnOk(WORD, UINT, HWND, BOOL&)
    {
        ::EndDialog(m_hWnd, IDOK);
        return 0;
    }

private:
    BITMAP      m_bm;
    CDCHandle   m_hdcMem;
    int         m_nAniType;
};

void gdi9()
{
    CGdi9Dlg dlg;
    dlg.DoModal();
}
