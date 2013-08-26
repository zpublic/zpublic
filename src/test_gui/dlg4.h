#pragma once
#include "dlg4_handler.h"
#include "resource.h"

const int DLG4_ID_EDIT = 10086;

class CDlg4
    : public CDialogImpl<CDlg4>
    , public CDlg4Handler<CDlg4>
{
public:
    enum
    {
        IDD = IDD_DIALOG4
    };

    BEGIN_MSG_MAP(CDlg4)
        CHAIN_MSG_MAP(CDlg4Handler<CDlg4>)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnOk)
        COMMAND_ID_HANDLER(IDC_BUTTON1, OnClickBtn1)
        ALT_MSG_MAP(DLG4_ID_EDIT)
            MESSAGE_HANDLER(WM_CHAR, OnEditChar)
    END_MSG_MAP()

private:
    BOOL OnInitDialog(CWindow wndFocus, LPARAM)
    {
        CRect rect(200,100,400,130);
        m_edit.Create(
            L"edit",
            this,
            DLG4_ID_EDIT,
            m_hWnd,
            &rect);
        CWindow btn1 = GetDlgItem(IDC_BUTTON1);
        btn1.EnableWindow(FALSE);
        return 1;
    }

    LRESULT OnOk(WORD, UINT, HWND, BOOL&)
    {
        EndDialog(IDOK);
        return 0;
    }

    LRESULT OnClickBtn1(WORD, UINT, HWND hwnd, BOOL&)
    {
        static int x = 0;
        CString strNum;
        strNum.Format(L"num=%d", ++x);
        CWindow static1 = GetDlgItem(IDC_STATIC1);
        static1.SetWindowText(strNum);
        CWindow btn1(hwnd);
        btn1.EnableWindow(FALSE);
        return 0;
    }

    LRESULT OnEditChar(UINT, WPARAM wparam, LPARAM, BOOL& bHandled)
    {
        if (isdigit((wchar_t)wparam))
        {
            bHandled = FALSE;
        }
        else
        {
            ::MessageBeep(0xffffffff);
        }
        return 0;
    }

private:
    CContainedWindow m_edit;
};