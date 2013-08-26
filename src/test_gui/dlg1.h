#pragma once

class CDlg1 : public CWindowImpl<CDlg1>
{
public:
    DECLARE_WND_CLASS(L"CDlg1")

    BEGIN_MSG_MAP(CDlg1)
        MSG_WM_CREATE(OnCreate)
    END_MSG_MAP()

private:
    int OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        ::MessageBox(0, L"OnCreate", L"", 0);
        return 0;
    }

    void OnFinalMessage(HWND hwnd)
    {
        ::PostQuitMessage(0);
    }
};