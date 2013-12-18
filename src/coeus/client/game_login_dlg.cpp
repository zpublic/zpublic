/* -------------------------------------------------------------------------
//  File        :   game_login_dlg.cpp
//  Author      :   Sum
//  Datet       :   2013/12/18 0:26
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "game_login_dlg.h"
#include "bkwinres.h"

#define WINDOW_MAX_WIDTH            460
#define WINDOW_MAX_HEIGHT           465

#define WINDOW_MIN_WIDTH            460
#define WINDOW_MIN_HEIGHT           55

#define TIMER_BUTTON_MOVE           1

#define NUM_BUFFER_MAX              260

LRESULT GameLoginDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
    LoadIcon(NULL, MAKEINTRESOURCE(IDI_MIN_MAIN));

    m_LoginNameEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD, 
        0, 400, NULL);
    m_LoginNameEdit.SetBgColor(RGB(255,255,255));
    m_LoginNameEdit.SetNotify(m_hWnd);

    m_PasswordEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD,
        0, 401, NULL);
    m_PasswordEdit.SetBgColor(RGB(255,255,255));
    m_PasswordEdit.SetNotify(m_hWnd);

    return TRUE;
}

void GameLoginDlg::OnClose()
{
    EndDialog(IDOK);
}

void GameLoginDlg::OnSysCommand(UINT nID, CPoint point)
{
    SetMsgHandled(FALSE);

    if (nID == SC_CLOSE)
    {
        EndDialog(IDOK);
    }
}

void GameLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
}

void GameLoginDlg::OnBtnClose()
{

}