#pragma once
#include "resource.h"

template <typename T>
class CDlg4Handler
{
public:
    BEGIN_MSG_MAP(CDlg4Handler<T>)
        COMMAND_ID_HANDLER(IDC_BUTTON2, OnClickBtn2)
    END_MSG_MAP()

private:
    LRESULT OnClickBtn2(WORD, UINT, HWND, BOOL&)
    {
        CWindow btn1 = static_cast<T*>(this)->GetDlgItem(IDC_BUTTON1);
        btn1.EnableWindow();
        return 0;
    }
};