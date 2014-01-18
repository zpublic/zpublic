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

void RegisterLogic::SendRegister( const CString& strUserName, const CString& csPassword )
{
    CSRegisterReq registerRequest;
    CStringA csPasswordA = CW2A(csPassword).m_psz;
    CStringA csUserNameA = CW2A(strUserName).m_psz;

    if (strUserName.IsEmpty()
        || csPasswordA.IsEmpty())
    {
        return;
    }

    registerRequest.username = csUserNameA;
    registerRequest.password = csPasswordA;
    NET.SendMessage(Opcodes::CSLoginReq, registerRequest);
}