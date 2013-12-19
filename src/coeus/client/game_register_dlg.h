/* -------------------------------------------------------------------------
//  File        :   game_register_dlg.h
//  Author      :   Sum
//  Datet       :   2013/12/19 22:55
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __GAME_REGISTER_DLG_H_
#define __GAME_REGISTER_DLG_H_

#include <vector>
#include <wtlhelper/whwindow.h>
#include <bkres/bkres.h>
#include "ktipedit3.h"

class GameRegisterDlg
    : public CBkDialogImpl<GameRegisterDlg>
    , public CWHRoundRectFrameHelper<GameRegisterDlg>
{
public:
    GameRegisterDlg()
        : CBkDialogImpl<GameRegisterDlg>(IDR_BK_REGISTER)
    {
    }

    enum
    {
        ID_MAIL_EDIT             = 4000,
        ID_NINCKNAME_EDIT        = 4001,
        ID_PASSWORD_EDIT         = 4002,
        ID_TRUEPASSSWORD_EDIT    = 4003,
        ID_CANCEL_BTN            = 10001,
        ID_REGISTER_BTN          = 10002,
    };

protected:

    LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);

    void OnClose();

    LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

    void OnSysCommand(UINT nID, CPoint point);

    void OnTimer(UINT_PTR nIDEvent);

    void OnBtnRegister();

    void OnBtnCancel();

protected:
    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
        BK_NOTIFY_ID_COMMAND(ID_CANCEL_BTN, OnBtnCancel)
        BK_NOTIFY_ID_COMMAND(ID_REGISTER_BTN, OnBtnRegister)
    BK_NOTIFY_MAP_END()

    BEGIN_MSG_MAP_EX(GameRegisterDlg)
        MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
        CHAIN_MSG_MAP(CBkDialogImpl<GameRegisterDlg>)
        CHAIN_MSG_MAP(CWHRoundRectFrameHelper<GameRegisterDlg>)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_TIMER(OnTimer)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

private:
    KTipEdit3 m_MailNameEdit;
    KTipEdit3 m_NickNameEdit;
    KTipEdit3 m_PasswordEdit;
    KTipEdit3 m_TruePasswordEdit;
};

#endif // __GAME_REGISTER_DLG_H_