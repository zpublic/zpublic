#include "stdafx.h"
#include "game_register_dlg.h"
#include "util_function.h"
#include "window_manager.h"

LRESULT GameRegisterDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
    LoadIcon(NULL, MAKEINTRESOURCE(IDI_MIN_MAIN));

    m_MailNameEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD, 
        0, ID_MAIL_EDIT, NULL);
    m_MailNameEdit.SetBgColor(RGB(255,255,255));
    m_MailNameEdit.SetColor(RGB(0,0,0));
    m_MailNameEdit.SetNotify(m_hWnd);

    m_PasswordEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD,
        0, ID_PASSWORD_EDIT, NULL);
    m_PasswordEdit.SetBgColor(RGB(255,255,255));
    m_PasswordEdit.SetColor(RGB(0,0,0));
    m_PasswordEdit.SetNotify(m_hWnd);

    m_TruePasswordEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD,
        0, ID_TRUEPASSSWORD_EDIT, NULL);
    m_TruePasswordEdit.SetBgColor(RGB(255,255,255));
    m_TruePasswordEdit.SetColor(RGB(0,0,0));
    m_TruePasswordEdit.SetNotify(m_hWnd);

    return TRUE;
}

void GameRegisterDlg::OnClose()
{
    OnBtnCancel();
}

LRESULT GameRegisterDlg::OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return TRUE;
}

void GameRegisterDlg::OnBtnRegister()
{
    CString csMailName;
    CString csPassword;
    CString csTruePassword;
    m_MailNameEdit.GetText(csMailName);
    m_PasswordEdit.GetText(csPassword);
    m_TruePasswordEdit.GetText(csTruePassword);

    if (csMailName.IsEmpty()
        || csPassword.IsEmpty()
        || csTruePassword.IsEmpty())
    {
        MessageBox(L"���벻��Ϊ��", L"ceous");
        return;
    }

    std::string mail = CW2A(csMailName, CP_UTF8);
    if (!Util::IsEmailValid(mail))
    {
        MessageBox(L"�����������Ϲ淶", L"ceous");
        return;
    }

    if (csTruePassword.Compare(csPassword) != 0)
    {
        MessageBox(L"����2�����벻һ��", L"ceous");
        return;
    }

    CStringA pass = CW2A(csPassword, CP_UTF8);
    GameLogic::regis.SetRegisterDlg(m_hWnd);
    GameLogic::regis.SendRegister(csMailName, Util::GetMd5Str(pass));
}

void GameRegisterDlg::OnSysCommand(UINT nID, CPoint point)
{
    SetMsgHandled(FALSE);

    if (nID == SC_CLOSE)
    {
        EndDialog(GWIN_ID_EXIT);
    }
}

void GameRegisterDlg::OnTimer(UINT_PTR nIDEvent)
{

}

void GameRegisterDlg::OnBtnCancel()
{
    EndDialog(GWIN_ID_EXIT);
}

LRESULT GameRegisterDlg::OnRegisterResult(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    switch (wParam)
    {
    case 1:
        {
            MessageBox(L"ע��ɹ�", L"ceous");
            EndDialog(GWIN_ID_SUCCESS_EXIT);
        }
        break;
    case 2:
        {
            MessageBox(L"ע��ʧ��", L"ceous");
        }
        break;
    default:
        {
            MessageBox(L"�쳣", L"ceous");
        }
    }
    return S_OK;
}
