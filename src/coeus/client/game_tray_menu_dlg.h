/* -------------------------------------------------------------------------
//  File        :   game_tray_menu_dlg.h
//  Author      :   Sum
//  Datet       :   2014/1/10 22:17
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __GAME_TRAY_MENU_DLG_H_
#define __GAME_TRAY_MENU_DLG_H_

#include <bkres/bkres.h>
#include <wtlhelper/whwindow.h>

class TrayMenuDlg
    : public CWHRoundRectFrameHelper<TrayMenuDlg>
{
public:
    TrayMenuDlg();

    ~TrayMenuDlg();

public:
    enum emResID
    {
        TRAYMENU_BTN_DLG_QUIT       = 103,
        TRAYMENU_BTN_DLG_CALLMAIN   = 100,

        TRAYMENU_ICON_DLG_QUIT      = 10004,
        TRAYMENU_ICON_DLG_CALLMAIN  = 10001,

        TRAYMENU_TEXT_DLG_QUIT      = 20004,
        TRAYMENU_TEXT_DLG_CALLMAIN  = 20001,
    };

    BOOL PopUp(HWND hPrentsWnd, CPoint point);

    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
        BK_NOTIFY_ID_COMMAND(TRAYMENU_BTN_DLG_QUIT, OnBtnQuit)
        BK_NOTIFY_ID_COMMAND(TRAYMENU_BTN_DLG_CALLMAIN, OnBtnCallMain)

        BK_NOTIFY_MOUSEHOVER_EX(TRAYMENU_BTN_DLG_QUIT, OnBtnQuitHover)
        BK_NOTIFY_MOUSELEAVE_EX(TRAYMENU_BTN_DLG_QUIT, OnBtnQuitLeave)

        BK_NOTIFY_MOUSEHOVER_EX(TRAYMENU_BTN_DLG_CALLMAIN, OnBtnCallMainHover)
        BK_NOTIFY_MOUSELEAVE_EX(TRAYMENU_BTN_DLG_CALLMAIN, OnBtnCallMainLeave)
    BK_NOTIFY_MAP_END()

    BEGIN_MSG_MAP_EX(TrayMenuDlg)
        MSG_WM_NCACTIVATE(OnNcActivate)
        MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
        CHAIN_MSG_MAP(CBkDialogImpl<TrayMenuDlg>)
        CHAIN_MSG_MAP(CWHRoundRectFrameHelper<TrayMenuDlg>)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_TIMER(OnTimer)
        MSG_WM_CLOSE(OnClose)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

protected:
    void OnTimer(UINT_PTR nIDEvent);

    void OnClose();

    BOOL OnInitDialog(HWND wParam, LPARAM lParam);

    BOOL OnNcActivate(BOOL bActive);

    void OnBtnQuit();

    void OnBtnCallMain();

    void OnBtnCallMainHover();

    void OnBtnCallMainLeave();

    void OnBtnQuitHover();

    void OnBtnQuitLeave();

private:
    void Hide();
    void CalcPosition(CPoint &point, int nWidth, int nHeight);

    HWND m_hPrentsWnd;
    BOOL m_bShowing;
};

#endif // __GAME_TRAY_MENU_DLG_H_