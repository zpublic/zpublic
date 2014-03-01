/* -------------------------------------------------------------------------
//  File        :   game_login_dlg.h
//  Author      :   Sum
//  Datet       :   2013/12/18 0:25
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __GAME_LOGIN_DLG_H_
#define __GAME_LOGIN_DLG_H_

#include <vector>
#include <wtlhelper/whwindow.h>
#include <bkres/bkres.h>
#include "ktipedit3.h"

class GameLoginDlg
    : public CBkDialogImpl<GameLoginDlg>
    , public CWHRoundRectFrameHelper<GameLoginDlg>
{
public:
    GameLoginDlg()
        : CBkDialogImpl<GameLoginDlg>(IDR_BK_LOGIN)
    {
    }

    enum
    {
        ID_CLOSE_BTN    = 403,
        ID_REGISTER_BTN = 10001,
        ID_LOGIN_BTN    = 10002,
    };

protected:

    LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);

    void OnClose();

    LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

    void OnSysCommand(UINT nID, CPoint point);

    void OnTimer(UINT_PTR nIDEvent);

    void OnBtnClose();

    void OnBtnRegister();

    void OnBtnLogin();

    LRESULT OnLoginResult(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
        BK_NOTIFY_ID_COMMAND(ID_CLOSE_BTN, OnBtnClose)
        BK_NOTIFY_ID_COMMAND(ID_REGISTER_BTN, OnBtnRegister)
        BK_NOTIFY_ID_COMMAND(ID_LOGIN_BTN, OnBtnLogin)
    BK_NOTIFY_MAP_END()

    BEGIN_MSG_MAP_EX(GameLoginDlg)
        MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
        CHAIN_MSG_MAP(CBkDialogImpl<GameLoginDlg>)
        CHAIN_MSG_MAP(CWHRoundRectFrameHelper<GameLoginDlg>)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_TIMER(OnTimer)
        MESSAGE_HANDLER(msg_login_result, OnLoginResult)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

private:
    BOOL _DisposeLoginErr(uint32 nErr);

private:
    KTipEdit3 m_LoginNameEdit;
    KTipEdit3 m_PasswordEdit;
};

#endif // __GAME_LOGIN_DLG_H_