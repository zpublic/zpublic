#pragma once
#include "resource.h"

class CGdip1 : public CSimpleDialog<IDD_DIALOG6>
{
public:
    BEGIN_MSG_MAP(CGdip1)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_PAINT(OnPaint)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

public:
    CGdip1()
    {

    }
    ~CGdip1()
    {

    }

private:
    BOOL OnInitDialog(CWindow, LPARAM)
    {
        CenterWindow();
        return TRUE;
    }

    void OnPaint(CDCHandle dc)
    {
        PAINTSTRUCT  ps;
        HDC hdc = BeginPaint(&ps);
        {
            Graphics graphics(hdc);
            Pen      pen(Color(255, 0, 0, 255));
            graphics.DrawLine(&pen, 0, 0, 200, 100);
        }
        EndPaint(&ps);
    }

    LRESULT OnOk(WORD, UINT, HWND, BOOL&)
    {
        ::EndDialog(m_hWnd, IDOK);
        return 0;
    }
};

void gdip1()
{
    CGdip1 dlg;
    dlg.DoModal();
}