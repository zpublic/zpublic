#pragma once

class CPlayerInfoDlg : public CDialogImpl<CPlayerInfoDlg>
{
public:
    enum { IDD = IDD_DIALOG_PLAYER_INFO };

    BEGIN_MSG_MAP(CPlayerInfoDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(msg_player_info, OnPlayerInfo);
    END_MSG_MAP()

private:
    void _GetPlayerInfo();

    LRESULT OnPlayerInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
    BOOL m_bWaiting;

private:
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};
