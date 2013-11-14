#include "stdafx.h"
#include "resource.h"

#include "World2Dlg.h"

LRESULT CWorld2Dlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());
    return TRUE;
}

LRESULT CWorld2Dlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    EndDialog(0);
    return 0;
}

LRESULT CWorld2Dlg::OnBnClickedW1Zhiliao(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    MessageBox(L"下一页还没写呢", L"下一页还没写呢");
    return 0;
}