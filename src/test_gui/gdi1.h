#pragma once
#include "resource.h"

class CGdi1Dlg : public CSimpleDialog<IDD_DIALOG_GDI1>
{
public:
    BEGIN_MSG_MAP(CGdi1Dlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CTLCOLORDLG(OnCtlColorDlg)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

public:
    CGdi1Dlg() : m_hBrush(NULL) {}
    ~CGdi1Dlg()
    {
        if (m_hBrush)
        {
            DeleteObject(m_hBrush);
        }
    }

private:
    BOOL OnInitDialog(CWindow, LPARAM)
    {
        assert(m_hBrush == NULL);
        HBITMAP hBitmap = (HBITMAP)LoadImage(
            g_module.m_hInst,
            MAKEINTRESOURCE(IDB_BITMAP1),
            IMAGE_BITMAP,
            0,
            0,
            LR_CREATEDIBSECTION);
        if (hBitmap)  
        {  
            m_hBrush = CreatePatternBrush(hBitmap);
        }
        
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
        return m_hBrush;
    }

private:
    HBRUSH m_hBrush;
};

class CGdi1DlgEx : public CSimpleDialog<IDD_DIALOG_GDI1>
{
public:
    BEGIN_MSG_MAP(CGdi1DlgEx)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CTLCOLORDLG(OnCtlColorDlg)
        MSG_WM_SIZE(OnSize)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

public:
    CGdi1DlgEx() : m_hdcMem(NULL) {}
    ~CGdi1DlgEx()
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

private:
    BITMAP m_bm;
    HDC    m_hdcMem;
};

void gdi1()
{
    CGdi1Dlg dlg;
    dlg.DoModal();

    CGdi1DlgEx dlg2;
    dlg2.DoModal();
}
