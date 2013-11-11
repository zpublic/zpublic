#include "stdafx.h"
#include "register_handler.h"

RegisterHandler::RegisterHandler()
{
    m_hRegisterDlg = NULL;
}

void RegisterHandler::SetRegisterDlg( HWND hWnd )
{
    m_hRegisterDlg = hWnd;
}

void RegisterHandler::NotifyResult()
{
    ::PostMessage(m_hRegisterDlg, msg_register_result, 1, 0);
}

void RegisterHandler::NotifyResult( std::string& strErr )
{
    ::SendMessage(m_hRegisterDlg, msg_register_result, 2, (LPARAM)strErr.c_str());
}

void RegisterHandler::SendRegister(
    const CString& email,
    const CStringA& pass,
    const CString& nickname,
    uint32 gender )
{
    Protocol::C2SRegisterReq reg;
    reg.set_email(CW2A(email, CP_UTF8));
    reg.set_password(pass);
    reg.set_nickname(CW2A(nickname, CP_UTF8));
    reg.set_gender(gender);
    NET.Send(Opcodes::C2SRegisterReq, reg);
}
