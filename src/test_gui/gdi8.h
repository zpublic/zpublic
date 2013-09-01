#pragma once
#include "resource.h"


const int INIT_TRANSPARENT3 = 200;

class CGdi8 : public CSimpleDialog<IDD_DIALOG_GDI4>
{
public:
    BEGIN_MSG_MAP(CGdi8)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CTLCOLORDLG(OnCtlColorDlg)
        MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
        MSG_WM_SIZE(OnSize)
        MSG_WM_HSCROLL(OnHScroll)
        MSG_WM_DRAWITEM(OnDrawItem)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

public:
    CGdi8() : m_hdcMem(NULL) {}
    ~CGdi8()
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
        ::SetLayeredWindowAttributes(m_hWnd, 0, INIT_TRANSPARENT3, LWA_ALPHA);

        ::SendMessage(GetDlgItem(IDC_SLIDER_TRANSPARENT), TBM_SETRANGE, (WPARAM)FALSE, MAKELONG(0, 255));
        ::SendMessage(GetDlgItem(IDC_SLIDER_TRANSPARENT), TBM_SETPOS, (WPARAM)TRUE, INIT_TRANSPARENT3);

        m_listBox.Attach(GetDlgItem(IDC_LIST1));
        m_listBox.AddString(L"愤怒的泡面1");
        m_listBox.AddString(L"愤怒的泡面2");
        m_listBox.AddString(L"愤怒的泡面3");
        m_listBox.AddString(L"愤怒的泡面4");
        m_listBox.AddString(L"愤怒的泡面5");

        CenterWindow();
        return TRUE;
    }

    void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
    {
        if (nIDCtl == IDC_LIST1)
        {
            int nItemId = lpDrawItemStruct->itemID;
            if (nItemId != -1)
            {
                CString strText;
                m_listBox.GetText(nItemId, strText);

                CDCHandle hdc(lpDrawItemStruct->hDC);
                if ((lpDrawItemStruct->itemState & ODS_SELECTED)
                    && (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
                {
                    hdc.DrawFocusRect(&lpDrawItemStruct->rcItem);
                    hdc.InvertRect(&lpDrawItemStruct->rcItem);
                }
                else if (!(lpDrawItemStruct->itemState & ODS_SELECTED)
                    && (lpDrawItemStruct->itemAction & ODA_SELECT))
                {
                    hdc.DrawFocusRect(&lpDrawItemStruct->rcItem);
                    hdc.InvertRect(&lpDrawItemStruct->rcItem);
                }
                if (lpDrawItemStruct->itemID % 2 == 0)
                    hdc.SetTextColor(RGB(25, 125, 0));
                else
                    hdc.SetTextColor(RGB(0, 25, 125));

                if (lpDrawItemStruct->itemState & ODS_SELECTED)
                {
                    hdc.SetBkMode(TRANSPARENT);
                    hdc.SetTextColor(RGB(255, 255, 255));
                }
                else
                {
                    hdc.SetBkMode(OPAQUE);
                    hdc.SetBkColor(GetSysColor(COLOR_WINDOW));
                }
                hdc.DrawText(strText, strText.GetLength(), &lpDrawItemStruct->rcItem, DT_LEFT | DT_SINGLELINE);
            }
        }
        else
        {
            SetMsgHandled(FALSE);
        }
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
        else
        {
            SetMsgHandled(FALSE);
        }
    }

    HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
    {
        if (wndStatic.GetDlgCtrlID() == IDC_STATIC1)
        {
            ::SetBkMode(dc, TRANSPARENT);
        }
        else
        {
            SetMsgHandled(FALSE);
        }
        return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }

private:
    BITMAP      m_bm;
    HDC         m_hdcMem;
    CListBox    m_listBox;
};

void gdi8()
{
    CGdi8 dlg;
    dlg.DoModal();
}
