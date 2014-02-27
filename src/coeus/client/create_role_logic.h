#pragma once

class CreateRoleLogic
{
public:
    CreateRoleLogic();

    void SetCreateRoleDlg(HWND hWnd);

    void SendCreateRole(const CString& strNickname, const uint8& gender, const uint8& belief, const uint8& characterType);
    void SendRandNickName(const uint8& gender, const uint8& type);
    void SendCheckNickName(const CString& strNickname);
	
    void NotifyRandNickNameSync(std::string const * nErr);
    void NotifyCheckNickName();
    void NotifyCheckNickNameSync(uint8 nErr);
    void NotifyCreateRoleResult();
    void NotifyCreateRoleResultSync(uint8 nErr);

private:
    HWND    m_hLoginDlg;
};
