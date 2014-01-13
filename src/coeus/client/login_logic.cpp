#include "stdafx.h"
#include "login_logic.h"

LoginLogic::LoginLogic()
{
    m_hLoginDlg = NULL;
}

void LoginLogic::SetLoginDlg( HWND hWnd )
{
    m_hLoginDlg = hWnd;
}

void LoginLogic::NotifyResult()
{
    ::PostMessage(m_hLoginDlg, msg_login_result, 1, 0);
}

void LoginLogic::NotifyResult( std::string& strErr )
{
    ::SendMessage(m_hLoginDlg, msg_login_result, 2, (LPARAM)strErr.c_str());
}

void LoginLogic::SendLogin( const CString& email, const CStringA& pass )
{
    CSLoginReq loginRequest;
    loginRequest.account = "coeus_user";
    loginRequest.password = pass;
    NET.SendMessage(Opcodes::CSLoginReq, loginRequest);
}
