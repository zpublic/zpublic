#pragma once
#include "resource.h"

class CDlgLu
    : public CDialogImpl<CDlgLu>
{
public:
    enum
    {
        IDD = IDD_DIALOG_LU
    };

    BEGIN_MSG_MAP(CDlgLu)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnOk)
        COMMAND_ID_HANDLER(IDC_BUTTON1, OnClickBtn1)
    END_MSG_MAP()

private:
    BOOL OnInitDialog(CWindow wndFocus, LPARAM)
    {
        CenterWindow();
        return 1;
    }

    LRESULT OnOk(WORD, UINT, HWND, BOOL&)
    {
        EndDialog(IDOK);
        return 0;
    }

    LRESULT OnClickBtn1(WORD, UINT, HWND hwnd, BOOL&)
    {
        return 0;
    }
};
