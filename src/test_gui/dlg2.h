#pragma once
#include "resource.h"

class CDlg2 : public CDialogImpl<CDlg2>
{
public:
    BEGIN_MSG_MAP(CDlg2)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

    enum
    {
        IDD = IDD_DIALOG2
    };

private:
    BOOL OnInitDialog(CWindow wndFocus, LPARAM)
    {
        ::MessageBox(0, L"OnInitDialog", L"", 0);
        return 1;
    }

    LRESULT OnOk(WORD, UINT, HWND, BOOL&)
    {
        EndDialog(IDOK);
        return 0;
    }
};