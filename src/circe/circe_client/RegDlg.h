#pragma once

class CRegDlg : public CDialogImpl<CRegDlg>
{
public:
	enum { IDD = IDD_REG_DLG };

	BEGIN_MSG_MAP(CRegDlg)
		MESSAGE_HANDLER(WM_INITDIALOG,  OnInitDialog)
        MESSAGE_HANDLER(msg_register_result, OnRegResult)
		COMMAND_ID_HANDLER(IDC_REG,     OnOk)
		COMMAND_ID_HANDLER(IDC_CANCEL,  OnCancel)
	END_MSG_MAP()

private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnRegResult(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

    CString m_strRegErr;
};
