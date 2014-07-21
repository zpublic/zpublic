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
            if (bRet)
            {
                UpdateUi(v);
            }
        }

        return 1;
    }

    void UpdateUi(Json::Value& v)
    {
        SetDlgItemText(IDC_STATIC_TITLE, CA2W(v["title"].asString().c_str(), CP_ACP));
        SetDlgItemText(IDC_STATIC_T1, CA2W(v["comment"]["1"].asString().c_str(), CP_ACP));
        SetDlgItemText(IDC_STATIC_T2, CA2W(v["comment"]["2"].asString().c_str(), CP_ACP));
        SetDlgItemText(IDC_STATIC_T3, CA2W(v["comment"]["3"].asString().c_str(), CP_ACP));
        SetDlgItemText(IDC_STATIC_T4, CA2W(v["comment"]["4"].asString().c_str(), CP_ACP));
        SetDlgItemText(IDC_STATIC_T5, CA2W(v["comment"]["5"].asString().c_str(), CP_ACP));
        SetDlgItemText(IDC_STATIC_T6, CA2W(v["comment"]["6"].asString().c_str(), CP_ACP));
        SetDlgItemText(IDC_STATIC_T7, CA2W(v["comment"]["7"].asString().c_str(), CP_ACP));
        SetDlgItemText(IDC_STATIC_T8, CA2W(v["comment"]["8"].asString().c_str(), CP_ACP));
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
