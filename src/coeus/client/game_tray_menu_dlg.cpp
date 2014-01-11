/* -------------------------------------------------------------------------
//  File        :   game_tray_menu_dlg.cpp
//  Author      :   Sum
//  Datet       :   2014/1/10 22:17
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "game_tray_menu_dlg.h"

#define TIMER_TOP               1

TrayMenuDlg::TrayMenuDlg()
    : CBkDialogImpl<TrayMenuDlg>(0)
    , m_bShowing(FALSE)
{
    m_hPrentsWnd = NULL;
}

TrayMenuDlg::~TrayMenuDlg()
{
}

BOOL TrayMenuDlg::PopUp(HWND hPrentsWnd, CPoint point)
{
    int nHeight = 140;
    int nWidth = 135;

    SetTimer(TIMER_TOP, 200);
    CalcPosition(point, nWidth, nHeight);
    SetWindowPos(HWND_TOPMOST, point.x, point.y, nWidth, nHeight, SWP_SHOWWINDOW);
    ::SetForegroundWindow(m_hWnd);

    m_hPrentsWnd = hPrentsWnd;

    if (!m_bShowing)
    {
        m_bShowing = TRUE;
    }

    return TRUE;
}

BOOL TrayMenuDlg::OnInitDialog(HWND wParam, LPARAM lParam)
{
    return TRUE;
}

BOOL TrayMenuDlg::OnNcActivate(BOOL bActive)
{
    if (!bActive && m_bShowing)
    {
        Hide();
    }
    return TRUE;
}

void TrayMenuDlg::Hide()
{
    m_bShowing = FALSE;
    ShowWindow(SW_HIDE);
    KillTimer(TIMER_TOP);
}

void TrayMenuDlg::CalcPosition(CPoint &point, int nWidth, int nHeight)
{
    // 获得屏幕分辨率
    int xScrn = GetSystemMetrics(SM_CXSCREEN);
    int yScrn = GetSystemMetrics(SM_CYSCREEN);
    const int n_margin = 4;

    if (point.x + nWidth + n_margin >= xScrn)
    {
        point.x -= nWidth;
        point.x -= n_margin;
    }
    else
    {
        point.x += n_margin;
    }

    if (point.y + nHeight + n_margin >= yScrn)
    {
        point.y -= nHeight;
        point.y -= n_margin;
    }
    else
    {
        point.y += n_margin;
    }
}

void TrayMenuDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case TIMER_TOP:
        {
            SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }
        break;
    }
}

void TrayMenuDlg::OnBtnQuit()
{
    if (!m_hPrentsWnd)
    {
        return;
    }

    ::SendMessage(m_hPrentsWnd, WM_GA_QUIT, NULL, NULL);
    EndDialog(TRUE);
}

void TrayMenuDlg::OnBtnCallMain()
{
    if (!m_hPrentsWnd)
    {
        return;
    }

    ::ShowWindow(m_hPrentsWnd, SW_SHOW);
    Hide();
}

void TrayMenuDlg::OnBtnQuitHover()
{
    SetItemAttribute(TRAYMENU_ICON_DLG_QUIT, "sub", "1");
    SetItemAttribute(TRAYMENU_TEXT_DLG_QUIT, "crtext", "FFFFFF");
}

void TrayMenuDlg::OnBtnQuitLeave()
{
    SetItemAttribute(TRAYMENU_ICON_DLG_QUIT, "sub", "2");
    SetItemAttribute(TRAYMENU_TEXT_DLG_QUIT, "crtext", "000000");
}

void TrayMenuDlg::OnBtnCallMainHover()
{
    SetItemAttribute(TRAYMENU_ICON_DLG_CALLMAIN, "sub", "1");
    SetItemAttribute(TRAYMENU_TEXT_DLG_CALLMAIN, "crtext", "FFFFFF");
}

void TrayMenuDlg::OnBtnCallMainLeave()
{
    SetItemAttribute(TRAYMENU_ICON_DLG_CALLMAIN, "sub", "2");
    SetItemAttribute(TRAYMENU_TEXT_DLG_CALLMAIN, "crtext", "000000");
}

void TrayMenuDlg::OnClose()
{
    KillTimer(TIMER_TOP);
}
