#pragma once

class RegisterHandler
{
public:
    RegisterHandler();

    void SetRegisterDlg(HWND hWnd);

    void SendRegister(
        const CString& email,
        const CStringA& pass,
        const CString& nickname,
        uint32 gender);

    void NotifyResult();
    void NotifyResult(std::string& strErr);

private:
    HWND    m_hRegisterDlg;
};
