#pragma once
#include "resource.h"


const int INIT_TRANSPARENT = 200;

class CGdi2 : public CSimpleDialog<IDD_DIALOG_GDI2>
{
public:
    BEGIN_MSG_MAP(CGdi2)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CTLCOLORDLG(OnCtlColorDlg)
        MSG_WM_SIZE(OnSize)
        MSG_WM_HSCROLL(OnHScroll)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

public:
    CGdi2() : m_hdcMem(NULL) {}
    ~CGdi2()
    {
        if (m_hdcMem)
        {
            ::DeleteDC(m_hdcMem);  
        }
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
            HDC hdc = ::GetDC(wnd);
            m_hdcMem = ::CreateCompatibleDC(hdc);
            ::SelectObject(m_hdcMem, hBitmap);
            ::ReleaseDC(wnd, hdc);
            ::GetObject(hBitmap, sizeof(m_bm), &m_bm); 
        }

        SetWindowLong(GWL_EXSTYLE, GetWindowLong(GWL_EXSTYLE) | WS_EX_LAYERED);   
        ::SetLayeredWindowAttributes(m_hWnd, 0, INIT_TRANSPARENT, LWA_ALPHA);  

        ::SendMessage(GetDlgItem(IDC_SLIDER_TRANSPARENT), TBM_SETRANGE, (WPARAM)FALSE, MAKELONG(0, 255));  
        ::SendMessage(GetDlgItem(IDC_SLIDER_TRANSPARENT), TBM_SETPOS, (WPARAM)TRUE, INIT_TRANSPARENT);

        CenterWindow();
        return TRUE;
    }

    LRESULT OnOk(WORD, UINT, HWND, BOOL&)
    {
        ::EndDialog(m_hWnd, IDOK);
        return 0;
    }

    HBRUSH OnCtlColorDlg(CDCHandle dc, CWindow wnd)
    {
        RECT rcDialog;
        GetClientRect(&rcDialog);

        ::SetStretchBltMode(dc, COLORONCOLOR);
        ::StretchBlt(dc, 0, 0, rcDialog.right, rcDialog.bottom, m_hdcMem, 0, 0, m_bm.bmWidth, m_bm.bmHeight, SRCCOPY);
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }

    void OnSize(UINT nType, CSize size)
    {
        Invalidate();
    }

    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
    {
        if (pScrollBar.GetDlgCtrlID() == IDC_SLIDER_TRANSPARENT)
        {
            int nTransparent = ::SendMessage(pScrollBar, TBM_GETPOS, 0, 0);
            ::SetLayeredWindowAttributes(m_hWnd, 0, nTransparent, LWA_ALPHA);
        }
    }

private:
    BITMAP m_bm;
    HDC    m_hdcMem;
};

void gdi2()
{
    CGdi2 dlg2;
    dlg2.DoModal();
}
