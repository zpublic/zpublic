#include "stdafx.h"
#include "login_handler.h"

LoginHandler::LoginHandler()
{
    m_hLoginDlg = NULL;
}

void LoginHandler::SetLoginDlg( HWND hWnd )
{
    m_hLoginDlg = hWnd;
}

void LoginHandler::NotifyResult()
{
    ::PostMessage(m_hLoginDlg, msg_login_result, 1, 0);
}

void LoginHandler::NotifyResult( std::string& strErr )
{
    ::SendMessage(m_hLoginDlg, msg_login_result, 2, (LPARAM)strErr.c_str());
}

void LoginHandler::SendLogin( const CString& email, const CStringA& pass )
{
    Protocol::C2SLoginReq login;
    login.set_email(CW2A(email, CP_UTF8));
    login.set_password(pass);
    NET.Send(Opcodes::C2SLoginReq, login);
}
