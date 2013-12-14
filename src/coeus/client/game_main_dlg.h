/* -------------------------------------------------------------------------
//  File        :   game_main_dlg.h
//  Author      :   Sum
//  Datet       :   2013/12/13 21:58
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __GAME_MAIN_DLG_H_
#define __GAME_MAIN_DLG_H_

#include <vector>
#include <wtlhelper/whwindow.h>
#include <bkres/bkres.h>

class GameMainDlg
    : public CBkDialogImpl<GameMainDlg>
    , public CWHRoundRectFrameHelper<GameMainDlg>
{
public:
    GameMainDlg()
        : CBkDialogImpl<GameMainDlg>(IDR_BK_MAIN_DIALOG)
    {
    }

    enum
    {

    };

protected:

    LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);

    void OnClose();

    LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

    void OnSysCommand(UINT nID, CPoint point);

    void OnTimer(UINT_PTR nIDEvent);

    void OnBtnClose();

protected:
    BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
        //BK_NOTIFY_ID_COMMAND(ID_CLOSE_BTN, OnBtnClose)
        //         BK_NOTIFY_ID_COMMAND(ID_OPEN_DLG, OnBtnOpenDlg)
        //         BK_NOTIFY_ID_COMMAND(ID_BTN_LOGIN, OnBtnLogin)
        //         BK_NOTIFY_ID_COMMAND(ID_BTN_RESET, OnBtnReset)
        BK_NOTIFY_MAP_END()

        BEGIN_MSG_MAP_EX(GameMainDlg)
        MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
        CHAIN_MSG_MAP(CBkDialogImpl<GameMainDlg>)
        CHAIN_MSG_MAP(CWHRoundRectFrameHelper<GameMainDlg>)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_TIMER(OnTimer)
        REFLECT_NOTIFICATIONS_EX()
        END_MSG_MAP()

private:

};

#endif // __GAME_MAIN_DLG_H_