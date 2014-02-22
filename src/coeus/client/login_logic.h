#pragma once

class LoginLogic
{
public:
    LoginLogic();

    void SetLoginDlg(HWND hWnd);

    void SendLogin(const CString& strUserName, const CStringA& csPassword);

    void NotifyResult();
    void NotifyResult(uint8 nErr);

private:
    HWND    m_hLoginDlg;
};
