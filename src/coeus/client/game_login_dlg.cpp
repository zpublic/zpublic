/* -------------------------------------------------------------------------
//  File        :   game_login_dlg.cpp
//  Author      :   Sum
//  Datet       :   2013/12/18 0:26
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "game_login_dlg.h"
#include "bkwinres.h"
#include "util_function.h"
#include "game_create_role_dlg.h"

LRESULT GameLoginDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
    LoadIcon(NULL, MAKEINTRESOURCE(IDI_MIN_MAIN));

    m_LoginNameEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD, 
        0, 400, NULL);
    m_LoginNameEdit.SetBgColor(RGB(255,255,255));
    m_LoginNameEdit.SetColor(RGB(0,0,0));
    m_LoginNameEdit.SetNotify(m_hWnd);

    m_PasswordEdit.SetStyle(ES_PASSWORD);
    m_PasswordEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD,
        0, 401, NULL);
    m_PasswordEdit.SetBgColor(RGB(255,255,255));
    m_PasswordEdit.SetColor(RGB(0,0,0));
    m_PasswordEdit.SetNotify(m_hWnd);

    return TRUE;
}

void GameLoginDlg::OnClose()
{
    EndDialog(IDCLOSE);
}

void GameLoginDlg::OnSysCommand(UINT nID, CPoint point)
{
    SetMsgHandled(FALSE);

    if (nID == SC_CLOSE)
    {
        EndDialog(IDCLOSE);
    }
}

void GameLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
}

void GameLoginDlg::OnBtnClose()
{
    EndDialog(IDCLOSE);
}

void GameLoginDlg::OnBtnRegister()
{
    ShowWindow(SW_HIDE);
    GameRegisterDlg dlg;
    dlg.DoModal();
    ShowWindow(SW_SHOW);
}

void GameLoginDlg::OnBtnLogin()
{
    CString csLoginName;
    CString csPassword;
    m_LoginNameEdit.GetText(csLoginName);
    m_PasswordEdit.GetText(csPassword);

    if (csLoginName.IsEmpty()
        || csPassword.IsEmpty())
    {
        MessageBox(L"ÕËºÅÃÜÂë²»ÄÜÎª¿Õ£¡", L"ceous");
        return;
    }

    std::string mail = CW2A(csLoginName, CP_UTF8);
    if (!Util::IsEmailValid(mail))
    {
        MessageBox(L"ÕÊºÅ²»·ûºÏ¹æ·¶£¨Ó¦ÎªÓÊÏäµØÖ·£©", L"ceous");
        return;
    }

    CStringA pass = CW2A(csPassword, CP_UTF8);
    GameLogic::login.SetLoginDlg(m_hWnd);
    GameLogic::login.SendLogin(csLoginName, Util::GetMd5Str(pass));
}

LRESULT GameLoginDlg::OnLoginResult(UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    BOOL& bHandled)
{
    if (uMsg == msg_login_result)
    {
        switch (wParam)
        {
        case 1:
            {
                ShowWindow(SW_HIDE);
                GameCreateRoleDlg dlg;
                dlg.DoModal();
                GameMainDlg dlgMain;
                dlgMain.DoModal();
                OnClose();
            }
            break;

        case 2:
            {
                _DisposeLoginErr(lParam);
            }
            break;

        default:
            break;
        }
    }

    return TRUE;
}

BOOL GameLoginDlg::_DisposeLoginErr(uint32 nErr)
{
    ::MessageBox(0, L"µÇÂ½Ê§°Ü", L"coeus", 0);
    return TRUE;
}
