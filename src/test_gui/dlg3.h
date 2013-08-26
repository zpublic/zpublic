#pragma once
#include "resource.h"

class CDlg3 : public CSimpleDialog<IDD_DIALOG3, TRUE>
{
public:
    BEGIN_MSG_MAP(CDlg3)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    END_MSG_MAP()

private:
    BOOL OnInitDialog(CWindow wndFocus, LPARAM)
    {
        ::MessageBox(0, L"OnInitDialog", L"", 0);
        return 1;
    }

    LRESULT OnCancel(WORD, UINT, HWND, BOOL&)
    {
        ::EndDialog(m_hWnd, IDCANCEL);
        return 0;
    }
};