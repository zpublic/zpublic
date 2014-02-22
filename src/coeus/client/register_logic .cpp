/* -------------------------------------------------------------------------
//  File        :   register_logic .cpp
//  Author      :   Sum
//  Datet       :   2014/1/17 22:27
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "register_logic.h"

RegisterLogic::RegisterLogic()
{
    m_hRegisterDlg = NULL;
}

void RegisterLogic::SetRegisterDlg( HWND hWnd )
{
    m_hRegisterDlg = hWnd;
}

void RegisterLogic::NotifyResult()
{
    ::PostMessage(m_hRegisterDlg, msg_register_result, 1, 0);
}

void RegisterLogic::NotifyResult( uint8 nErr )
{
    ::SendMessage(m_hRegisterDlg, msg_register_result, 2, (LPARAM)nErr);
}

void RegisterLogic::SendRegister( const CString& strUserName, const CStringA& csPassword )
{
    CSRegisterReq registerRequest;
    CStringA csUserNameA = CW2A(strUserName).m_psz;

    if (strUserName.IsEmpty()
        || csPassword.IsEmpty())
    {
        return;
    }

    registerRequest.username = csUserNameA;
    registerRequest.password = csPassword;
    NET.SendMessage(Opcodes::CSRegisterReq, registerRequest);
}