#pragma once
#include "resource.h"
#include "z_http/z_http.h"
using namespace zl::http;

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

        ZLHttpClient h;
        ZLMemWrite m;
        if (h.DownloadMem(L"http://www.laorouji.com/teapot/data.json", &m) == 0)
        {
            Json::Value v;
            Json::Reader reader;
            bool bRet = reader.parse((const char *)m.GetData(), (const char *)m.GetData() + m.GetLength(), v);
            bRet = true;
        }

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
