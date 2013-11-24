#pragma once

class PlayerInfoHandler
{
public:
    PlayerInfoHandler();

    void SetDlg(HWND hWnd);

    void SendMsg();

    void NotifyResult(Protocol::S2CGetPlayerProfileRsp& msg);

private:
    HWND    m_hDlg;
};
