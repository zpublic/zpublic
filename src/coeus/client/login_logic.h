#pragma once

class LoginLogic
{
public:
    LoginLogic();

    void SetLoginDlg(HWND hWnd);

    void SendLogin(const CString& email, const CStringA& pass);

    void NotifyResult();
    void NotifyResult(std::string& strErr);

private:
    HWND    m_hLoginDlg;
};
