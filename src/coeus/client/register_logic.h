/* -------------------------------------------------------------------------
//  File        :   register_logic.h
//  Author      :   Sum
//  Datet       :   2014/1/17 22:26
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __REGISTER_LOGIC_H_
#define __REGISTER_LOGIC_H_

class RegisterLogic
{
public:
    RegisterLogic();
    void SetRegisterDlg(HWND hWnd);

    void SendRegister(const CString& strUserName, const CString& csPassword);

    void NotifyResult();
    void NotifyResult(uint8 nErr);

private:
    HWND    m_hRegisterDlg;
};

#endif // __REGISTER_LOGIC_H_