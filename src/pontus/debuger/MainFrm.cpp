// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "debugerView.h"
#include "ChildFrm.h"
#include "MainFrm.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    if(CMDIFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
        return TRUE;

    HWND hWnd = MDIGetActive();
    if(hWnd != NULL)
        return (BOOL)::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);

    return FALSE;
}

BOOL CMainFrame::OnIdle()
{
    UIUpdateToolBar();
    return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
// 	// create command bar window
// 	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
// 	// attach menu
// 	m_CmdBar.AttachMenu(GetMenu());
// 	// load command bar images
// 	m_CmdBar.LoadImages(IDR_MAINFRAME);
// 	// remove old menu
// 	SetMenu(NULL);
// 
// 	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
// 
// 	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
// 	AddSimpleReBarBand(hWndCmdBar);
// 	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

    //CreateSimpleStatusBar();

    CreateMDIClient();
    //m_CmdBar.SetMDIClient(m_hWndMDIClient);

// 	UIAddToolBar(hWndToolBar);
// 	UISetCheck(ID_VIEW_TOOLBAR, 1);
// 	UISetCheck(ID_VIEW_STATUS_BAR, 1);

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
        //m_CmdBar.AttachMenu(NULL);

    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    bHandled = FALSE;
    return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    PostMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CChildFrame* pChild = new CChildFrame;
    pChild->CreateEx(m_hWndClient);

    // TODO: add code to initialize document

    return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    static BOOL bVisible = TRUE;	// initially visible
    bVisible = !bVisible;
    CReBarCtrl rebar = m_hWndToolBar;
    int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
    rebar.ShowBand(nBandIndex, bVisible);
    UISetCheck(ID_VIEW_TOOLBAR, bVisible);
    UpdateLayout();
    return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
    ::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
    UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
    UpdateLayout();
    return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CAboutDlg dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainFrame::OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    MDICascade();
    return 0;
}

LRESULT CMainFrame::OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    MDITile();
    return 0;
}

LRESULT CMainFrame::OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    MDIIconArrange();
    return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() == IDOK)
    {
        m_dbg.AddObserver(static_cast<DbgEventObserver*>(this));
        m_dbg.StartDebug(dlg.m_szFileName);
    }
    return 0;
}

void CMainFrame::OnProcessExited( const EXIT_PROCESS_DEBUG_INFO* )
{
    ::MessageBox(0, L"exit", L"hehe", MB_OK);
}

void CMainFrame::OnDllLoaded( const LOAD_DLL_DEBUG_INFO* pInfo)
{

}

void CMainFrame::OnDllUnloaded( const UNLOAD_DLL_DEBUG_INFO* pInfo)
{

    //::MessageBox(0, param->lpBaseOfDll, L"load", MB_OK);
}
