#pragma once
#include "resource.h"
#include "lab/publish/gdiplus_resource.h"

class CGdip3 : public CSimpleDialog<IDD_DIALOG6>
{
public:
    BEGIN_MSG_MAP(CGdip3)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_PAINT(OnPaint)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

public:
    CGdip3()
    {

    }
    ~CGdip3()
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
        CDC hdc = BeginPaint(&ps);
        {
            Graphics    graphics(hdc);
            Image       *image = LoadSkinImage(IDB_PNG1);
            if (image)
            {
                TextureBrush  tBrush(image);
                Pen           texturedPen(&tBrush, 30);
                //graphics.DrawImage(image, 0, 0, image->GetWidth(), image->GetHeight());
                //graphics.DrawEllipse(&texturedPen, 150, 20, 400, 100);

                CRect rect;
                GetClientRect(rect);
                graphics.DrawRectangle(&texturedPen, 0, 0, rect.Width(), rect.Height());
                delete image;
            }
        }
        EndPaint(&ps);
    }

    LRESULT OnOk(WORD, UINT, HWND, BOOL&)
    {
        ::EndDialog(m_hWnd, IDOK);
        return 0;
    }
};

void gdip3()
{
    CGdip3 dlg;
    dlg.DoModal();
}