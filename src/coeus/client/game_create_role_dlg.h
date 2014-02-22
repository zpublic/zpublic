#ifndef __GAME_CREATE_ROLE_DLG_H_
#define __GAME_CREATE_ROLE_DLG_H_

#include <vector>
#include <wtlhelper/whwindow.h>
#include <bkres/bkres.h>
#include "ktipedit3.h"
#include "game_register_dlg.h"
#include "game_main_dlg.h"

class GameCreateRoleDlg
    : public CBkDialogImpl<GameCreateRoleDlg>
    , public CWHRoundRectFrameHelper<GameCreateRoleDlg>
{
public:
    GameCreateRoleDlg()
        : CBkDialogImpl<GameCreateRoleDlg>(IDR_BK_CREATEROLE)
    {
    }

    enum
    {
        ID_CLOSE_BTN    = 403,
        ID_SELECT_HUNTSMAN_MAN_BTN      = 60001,
        ID_SELECT_HUNTSMAN_WOMAN_BTN    = 60002,
        ID_SELECT_MECHANICIAN_MAN_BTN   = 60003,
        ID_SELECT_MECHANICIAN_WOMAN_BTN = 60004,
        ID_OK_BTN                       = 60007,
        ID_ROLE_IMG                     = 10000,
        ID_ROLE_NAME                    = 20001,
        ID_ROLE_EXPLAIN                 = 20002,
        ID_ROLE_EXPLAIN_DLG             = 20003,
    };

protected:

    LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);

    void OnClose();

    LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

    void OnSysCommand(UINT nID, CPoint point);

    void OnTimer(UINT_PTR nIDEvent);

    void OnBtnClose();

    void OnBtnOK();

    void OnSelectHuntsmanMan();

    void OnSelectHuntsmanWoMan();

    void OnSelectMechanicianMan();

    void OnSelectMechanicianWoMan();

    void SelectRole(UINT nBtnId, UINT nImgId, const CStringA& csSkinName);

    LRESULT OnRoleResult(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
        BK_NOTIFY_ID_COMMAND(ID_CLOSE_BTN, OnBtnClose)
        BK_NOTIFY_ID_COMMAND(ID_OK_BTN, OnBtnOK)
        BK_NOTIFY_ID_COMMAND(ID_SELECT_HUNTSMAN_MAN_BTN, OnSelectHuntsmanMan)
        BK_NOTIFY_ID_COMMAND(ID_SELECT_HUNTSMAN_WOMAN_BTN, OnSelectHuntsmanWoMan)
        BK_NOTIFY_ID_COMMAND(ID_SELECT_MECHANICIAN_MAN_BTN, OnSelectMechanicianMan)
        BK_NOTIFY_ID_COMMAND(ID_SELECT_MECHANICIAN_WOMAN_BTN, OnSelectMechanicianWoMan)
        BK_NOTIFY_MAP_END()

        BEGIN_MSG_MAP_EX(GameCreateRoleDlg)
        MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
        CHAIN_MSG_MAP(CBkDialogImpl<GameCreateRoleDlg>)
        CHAIN_MSG_MAP(CWHRoundRectFrameHelper<GameCreateRoleDlg>)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_TIMER(OnTimer)
        MESSAGE_HANDLER(msg_login_result, OnRoleResult)
        REFLECT_NOTIFICATIONS_EX()
        END_MSG_MAP()
private:
    KTipEdit3 m_NickNameEdit;
};
#endif // __GAME_CREATE_ROLE_DLG_H_