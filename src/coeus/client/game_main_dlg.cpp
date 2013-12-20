/* -------------------------------------------------------------------------
//  File        :   game_main_dlg.cpp
//  Author      :   Sum
//  Datet       :   2013/12/13 21:58
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "game_main_dlg.h"
#include "bkwinres.h"

#define WINDOW_MAX_WIDTH            460
#define WINDOW_MAX_HEIGHT           465

#define WINDOW_MIN_WIDTH            460
#define WINDOW_MIN_HEIGHT           55

#define TIMER_BUTTON_MOVE           1

#define NUM_BUFFER_MAX              260

LRESULT GameMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
    LoadIcon(NULL, MAKEINTRESOURCE(IDI_MIN_MAIN));

    return TRUE;
}

void GameMainDlg::OnClose()
{
    OnBtnClose();
}

void GameMainDlg::OnSysCommand(UINT nID, CPoint point)
{
    SetMsgHandled(FALSE);

    if (nID == SC_CLOSE)
    {
        EndDialog(IDOK);
    }
}

void GameMainDlg::OnTimer(UINT_PTR nIDEvent)
{
}

void GameMainDlg::OnBtnClose()
{
    EndDialog(IDOK);
}