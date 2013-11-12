#pragma once

class CWorldDlg : public CDialogImpl<CWorldDlg>
{
public:
    enum { IDD = IDD_WORLD };

    BEGIN_MSG_MAP(CWorldDlg)
        MESSAGE_HANDLER(WM_INITDIALOG,  OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_HANDLER(IDC_JUMP, BN_CLICKED, OnBnClickedJump)
        COMMAND_HANDLER(IDC_WORLD_1, BN_CLICKED, OnBnClickedWorld1)
    END_MSG_MAP()

private:
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnBnClickedJump(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
    LRESULT OnBnClickedWorld1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
