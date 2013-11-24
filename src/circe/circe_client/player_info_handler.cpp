#include "stdafx.h"
#include "player_info_handler.h"

PlayerInfoHandler::PlayerInfoHandler()
{
    m_hDlg = NULL;
}

void PlayerInfoHandler::SetDlg( HWND hWnd )
{
    m_hDlg = hWnd;
}

void PlayerInfoHandler::NotifyResult(Protocol::S2CGetPlayerProfileRsp& msg)
{
    ::SendMessage(m_hDlg, msg_player_info, (WPARAM)&msg, 0);
}

void PlayerInfoHandler::SendMsg()
{
    Protocol::C2SGetPlayerProfileReq msg;
    NET.Send(Opcodes::C2SGetPlayerProfileReq, msg);
}
