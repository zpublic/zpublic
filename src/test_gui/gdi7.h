#pragma once
#include "resource.h"

class CGdi7Dlg : public CSimpleDialog<IDD_DIALOG_GDI1>
{
public:
    BEGIN_MSG_MAP(CGdi7Dlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CTLCOLORDLG(OnCtlColorDlg)
        MSG_WM_SIZE(OnSize)
        MSG_WM_DROPFILES(OnDropFiles)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

public:
    CGdi7Dlg() : m_hdcMem(NULL) {}
    ~CGdi7Dlg()
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
            _Module.m_hInst,
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

        DragAcceptFiles();
        return TRUE;
    }

    void OnDropFiles(HDROP hDropInfo)
    {
        UINT nFileNum = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
        wchar_t strFileName[MAX_PATH];
        for (UINT i = 0; i < nFileNum; i++)
        {
            ::DragQueryFile(hDropInfo, i, strFileName, MAX_PATH);
            ::MessageBox(0, strFileName, 0, 0);
        }
        ::DragFinish(hDropInfo);
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

void gdi7()
{
    CGdi7Dlg dlg;
    dlg.DoModal();
}
