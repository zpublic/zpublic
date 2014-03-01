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
#include "util_function.h"
#include "window_manager.h"

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
    m_LoginNameEdit.SetColor(RGB(0,0,0));
    m_LoginNameEdit.SetNotify(m_hWnd);

    m_PasswordEdit.SetStyle(ES_PASSWORD);
    m_PasswordEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD,
        0, 401, NULL);
    m_PasswordEdit.SetBgColor(RGB(255,255,255));
    m_PasswordEdit.SetColor(RGB(0,0,0));
    m_PasswordEdit.SetNotify(m_hWnd);

    return TRUE;
}

void GameLoginDlg::OnClose()
{
    EndDialog(GWIN_ID_EXIT);
}

void GameLoginDlg::OnSysCommand(UINT nID, CPoint point)
{
    SetMsgHandled(FALSE);

    if (nID == SC_CLOSE)
    {
        EndDialog(GWIN_ID_EXIT);
    }
}

void GameLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
}

void GameLoginDlg::OnBtnClose()
{
    EndDialog(GWIN_ID_EXIT);
}

void GameLoginDlg::OnBtnRegister()
{
    ShowWindow(SW_HIDE);
    UINT_PTR uRet = GameWindowControl::start(GWIN_ID_REGISTER);
    if(uRet==GWIN_ID_SUCCESS_EXIT){
        GameWindowControl::destroy_register_dialog();
        GameWindowControl::get_register_dialog();
    }
    ShowWindow(SW_SHOW);
}

void GameLoginDlg::OnBtnLogin()
{
    CString csLoginName;
    CString csPassword;
    m_LoginNameEdit.GetText(csLoginName);
    m_PasswordEdit.GetText(csPassword);

    if (csLoginName.IsEmpty()
        || csPassword.IsEmpty())
    {
        MessageBox(L"ÕËºÅÃÜÂë²»ÄÜÎª¿Õ£¡", L"ceous");
        return;
    }

    std::string mail = CW2A(csLoginName, CP_UTF8);
    if (!Util::IsEmailValid(mail))
    {
        MessageBox(L"ÕÊºÅ²»·ûºÏ¹æ·¶£¨Ó¦ÎªÓÊÏäµØÖ·£©", L"ceous");
        return;
    }

    CStringA pass = CW2A(csPassword, CP_UTF8);
    GameLogic::login.SetLoginDlg(m_hWnd);
    GameLogic::login.SendLogin(csLoginName, Util::GetMd5Str(pass));
}

LRESULT GameLoginDlg::OnLoginResult(UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    BOOL& bHandled)
{
    switch (wParam)
    {
    case 1:
        {
            EndDialog(GWIN_ID_CREATE_ROLE);
        }
        break;

    case 2:
        {
            _DisposeLoginErr(lParam);
        }
        break;

    default:
        break;
    }

    return TRUE;
}

BOOL GameLoginDlg::_DisposeLoginErr(uint32 nErr)
{
    ::MessageBox(0, L"µÇÂ½Ê§°Ü", L"coeus", 0);
    return TRUE;
}
