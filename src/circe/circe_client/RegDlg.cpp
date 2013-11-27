#include "stdafx.h"
#include "resource.h"
#include "RegDlg.h"
#include "game_handler.h"
#include "utils.h"

LRESULT CRegDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
    CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	return TRUE;
}

LRESULT CRegDlg::OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CString email,pass,nickname;
    GetDlgItemText(IDC_EDIT1, email);
    GetDlgItemText(IDC_EDIT2, pass);
    GetDlgItemText(IDC_EDIT3, nickname);
    if (email.IsEmpty() || pass.IsEmpty() || nickname.IsEmpty())
    {
        MessageBox(L"有地方没填！");
        return 0;
    }
    //判断邮箱帐号非法
    std::string sEmail(CW2A(email, CP_UTF8));
    if (IsEmaiValid(sEmail) == false)
    {
        MessageBox(L"邮箱帐号非法! ");
        return 0;
    }

    uint32 gender = IsDlgButtonChecked(IDC_RADIO1) ? 1 : 2;
    GameHandler::reg.SetRegisterDlg(m_hWnd);
    CStringA sPass = CW2A(pass);
    GameHandler::reg.SendRegister(email, GetMd5Str(sPass), nickname, gender);
    ::EnableWindow(GetDlgItem(IDC_REG), FALSE);
    ::EnableWindow(GetDlgItem(IDC_CANCEL), FALSE);
    return 0;
}

LRESULT CRegDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}

LRESULT CRegDlg::OnRegResult(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    int nResult = (int)wParam;
    if (nResult == 1)
    {
        MessageBox(L"注册成功");
        EndDialog(0);
    }
    else if (nResult == 2)
    {
        LPCSTR lpErr = (LPCSTR)lParam;
        m_strRegErr = CA2W(lpErr);
        PostMessage(msg_register_result, 3);
    }
    else if (nResult == 3)
    {
        MessageBox(m_strRegErr);
        ::EnableWindow(GetDlgItem(IDC_REG), TRUE);
        ::EnableWindow(GetDlgItem(IDC_CANCEL), TRUE);
    }

    return 0;
}
