/* -------------------------------------------------------------------------
//  File        :   game_register_dlg.cpp
//  Author      :   Sum
//  Datet       :   2013/12/19 22:55
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "game_register_dlg.h"

LRESULT GameRegisterDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
    LoadIcon(NULL, MAKEINTRESOURCE(IDI_MIN_MAIN));

    m_MailNameEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD, 
        0, ID_MAIL_EDIT, NULL);
    m_MailNameEdit.SetBgColor(RGB(255,255,255));
    m_MailNameEdit.SetNotify(m_hWnd);

    m_NickNameEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD,
        0, ID_NINCKNAME_EDIT, NULL);
    m_NickNameEdit.SetBgColor(RGB(255,255,255));
    m_NickNameEdit.SetNotify(m_hWnd);

    m_PasswordEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD,
        0, ID_PASSWORD_EDIT, NULL);
    m_PasswordEdit.SetBgColor(RGB(255,255,255));
    m_PasswordEdit.SetNotify(m_hWnd);

    m_TruePasswordEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD,
        0, ID_TRUEPASSSWORD_EDIT, NULL);
    m_TruePasswordEdit.SetBgColor(RGB(255,255,255));
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

}

void GameRegisterDlg::OnSysCommand(UINT nID, CPoint point)
{
    SetMsgHandled(FALSE);

    if (nID == SC_CLOSE)
    {
        EndDialog(IDCLOSE);
    }
}

void GameRegisterDlg::OnTimer(UINT_PTR nIDEvent)
{

}

void GameRegisterDlg::OnBtnCancel()
{
    EndDialog(IDCLOSE);
}
