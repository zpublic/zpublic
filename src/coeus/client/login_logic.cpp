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

void LoginLogic::NotifyResult( uint8 nErr )
{
    ::SendMessage(m_hLoginDlg, msg_login_result, 2, (LPARAM)nErr);
}

void LoginLogic::SendLogin(const CString& strUserName, const CString& csPassword)
{
    CSLoginReq loginRequest;
    CStringA csPasswordA = CW2A(csPassword).m_psz;
    CStringA csUserNameA = CW2A(strUserName).m_psz;

    if (csUserNameA.IsEmpty()
        || csPasswordA.IsEmpty())
    {
        return;
    }

    loginRequest.account = csUserNameA;
    loginRequest.password = csPasswordA;
    NET.SendMessage(Opcodes::CSLoginReq, loginRequest);
}
