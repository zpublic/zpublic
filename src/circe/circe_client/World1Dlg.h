#pragma once

class CWorld1Dlg : public CDialogImpl<CWorld1Dlg>
{
public:
    enum { IDD = IDD_WORLD_1 };

    BEGIN_MSG_MAP(CWorld1Dlg)
        MESSAGE_HANDLER(WM_INITDIALOG,  OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_HANDLER(IDC_W1_ZHILIAO, BN_CLICKED, OnBnClickedW1Zhiliao)
    END_MSG_MAP()

private:
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnBnClickedW1Zhiliao(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    int     m_nMoney;
};
