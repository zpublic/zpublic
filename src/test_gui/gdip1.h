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
            graphics.DrawRectangle(&pen, 100, 100, 100, 50);

            Pen greenPen(Color(255, 0, 255, 0), 10);
            greenPen.SetAlignment(PenAlignmentCenter);
            graphics.DrawLine(&greenPen, 10, 100, 100, 50);

            greenPen.SetStartCap(LineCapArrowAnchor);
            greenPen.SetEndCap(LineCapRoundAnchor);
            graphics.DrawLine(&greenPen, 500, 175, 50, 300);

            GraphicsPath path;
            Pen penJoin(Color(255, 0, 0, 255), 8);
            path.StartFigure();
            path.AddLine(Point(50, 200), Point(100, 200));
            path.AddLine(Point(100, 200), Point(100, 250));
            penJoin.SetLineJoin(LineJoinBevel);
            graphics.DrawPath(&penJoin, &path);
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