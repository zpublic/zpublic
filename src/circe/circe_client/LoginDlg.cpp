// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "RegDlg.h"
#include "LoginDlg.h"
#include "game_handler.h"
#include "utils.h"

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CLoginDlg::OnIdle()
{
	return FALSE;
}

LRESULT CLoginDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

    HWND hWnd = GetDlgItem(IDC_GIF1);
    if (hWnd)
    {
        m_gifLogo.SubclassWindow(hWnd);
        if (m_gifLogo.Load(MAKEINTRESOURCE(IDR_GIF1),_T("GIF")))
            m_gifLogo.Draw();
    }

	return TRUE;
}

LRESULT CLoginDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CLoginDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CRegDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CLoginDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CString strEmail, strPass;
    GetDlgItemText(IDC_EDIT1, strEmail);
    GetDlgItemText(IDC_EDIT2, strPass);
    if (strEmail.IsEmpty() || strPass.IsEmpty())
    {
        MessageBox(L"¬È∑≥ÃÓ∫√’ ∫≈√‹¬Î£°");
        return 0;
    }
    GameHandler::login.SetLoginDlg(m_hWnd);
    CStringA pass = CW2A(strPass);
    GameHandler::login.SendLogin(strEmail, GetMd5Str(pass));

	//CloseDialog(wID);
	return 0;
}

LRESULT CLoginDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CLoginDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CLoginDlg::OnLoginResult(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    int nResult = (int)wParam;
    if (nResult == 1)
    {
        MessageBox(L"µ«¬Ω≥…π¶");
    }
    else if (nResult == 2)
    {
        LPCSTR lpErr = (LPCSTR)lParam;
        m_strLoginErr = CA2W(lpErr);
        PostMessage(msg_login_result, 3);
    }
    else if (nResult == 3)
    {
        MessageBox(m_strLoginErr);
    }

    return 0;
}