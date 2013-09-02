#pragma once
#include "resource.h"
#include "lab/3rd/PictureExWnd.h"

class CLab1 : public CDialogImpl<CLab1>
{
public:
    BEGIN_MSG_MAP(CLab1)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnOk)
    END_MSG_MAP()

    enum
    {
        IDD = IDD_DIALOG5
    };

private:
    BOOL OnInitDialog(CWindow wndFocus, LPARAM)
    {
        HWND hWnd = GetDlgItem(IDC_GIF1);
        if (hWnd)
        {
            m_wndBanner1.SubclassWindow(hWnd);
            if (m_wndBanner1.Load(MAKEINTRESOURCE(IDR_GIF1),_T("GIF")))
                m_wndBanner1.Draw();
        }
        hWnd = GetDlgItem(IDC_GIF2);
        if (hWnd)
        {
            m_wndBanner2.SubclassWindow(hWnd);
            if (m_wndBanner2.Load(MAKEINTRESOURCE(IDR_GIF1),_T("GIF")))
                m_wndBanner2.Draw();
        }
        hWnd = GetDlgItem(IDC_GIF3);
        if (hWnd)
        {
            m_wndBanner3.SubclassWindow(hWnd);
            if (m_wndBanner3.Load(MAKEINTRESOURCE(IDR_GIF1),_T("GIF")))
                m_wndBanner3.Draw();
        }
        hWnd = GetDlgItem(IDC_GIF4);
        if (hWnd)
        {
            m_wndBanner4.SubclassWindow(hWnd);
            if (m_wndBanner4.Load(MAKEINTRESOURCE(IDR_GIF1),_T("GIF")))
                m_wndBanner4.Draw();
        }
        hWnd = GetDlgItem(IDC_GIF5);
        if (hWnd)
        {
            m_wndBanner5.SubclassWindow(hWnd);
            if (m_wndBanner5.Load(MAKEINTRESOURCE(IDR_GIF1),_T("GIF")))
                m_wndBanner5.Draw();
        }
        return TRUE;
    }

    LRESULT OnOk(WORD, UINT, HWND, BOOL&)
    {
        EndDialog(IDOK);
        return 0;
    }

private:
    CPictureExWnd m_wndBanner1;
    CPictureExWnd m_wndBanner2;
    CPictureExWnd m_wndBanner3;
    CPictureExWnd m_wndBanner4;
    CPictureExWnd m_wndBanner5;
};

void Lab1()
{
    CLab1 dlg;
    dlg.DoModal();
}