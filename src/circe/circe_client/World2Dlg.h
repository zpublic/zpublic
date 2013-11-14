#pragma once

class CWorld2Dlg : public CDialogImpl<CWorld2Dlg>
{
public:
    enum { IDD = IDD_WORLD_2 };

    BEGIN_MSG_MAP(CWorld2Dlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_HANDLER(IDC_W2_HEHE, BN_CLICKED, OnBnClickedW1Zhiliao)
    END_MSG_MAP()

private:
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnBnClickedW1Zhiliao(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
    LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
