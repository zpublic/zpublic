#include "stdafx.h"
#include "heartbeat_handler.h"

HearbeatHandler::HearbeatHandler()
    : m_hGameDlg(NULL)
{

}

void HearbeatHandler::SetGameDlg(HWND hWnd)
{
    m_hGameDlg = hWnd;
}

void HearbeatHandler::NotifyResult()
{
    Protocol::C2SHeartbeat heartbeat;
    NET.Send(Opcodes::C2SHeartbeat, heartbeat);
}

void HearbeatHandler::NotifyResult(std::string& strErr)
{

}

void HearbeatHandler::SendHearbeat()
{
    NotifyResult();
}
