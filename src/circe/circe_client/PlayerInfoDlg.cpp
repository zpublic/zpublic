#include "stdafx.h"
#include "resource.h"

#include "PlayerInfoDlg.h"
#include "game_handler.h"

LRESULT CPlayerInfoDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());
    m_bWaiting = TRUE;

    return TRUE;
}

LRESULT CPlayerInfoDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    EndDialog(0);
    return 0;
}

void CPlayerInfoDlg::_GetPlayerInfo()
{
    GameHandler::player_info.SetDlg(m_hWnd);
    GameHandler::player_info.SendMsg();
}

LRESULT CPlayerInfoDlg::OnPlayerInfo(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (m_bWaiting)
    {
        Protocol::S2CGetPlayerProfileRsp* pMsg = reinterpret_cast<Protocol::S2CGetPlayerProfileRsp *>(wParam);
        SetDlgItemText(IDC_PLAYER_INFO_1, CA2W(pMsg->nickname().c_str(), CP_UTF8));
        SetDlgItemText(IDC_PLAYER_INFO_2, (pMsg->gender() == 1) ? L"boy" : L"girl");
        pMsg->last_login();
        SetDlgItemText(IDC_PLAYER_INFO_3, L"懒得转，等别人写吧 todo Todo");
        m_bWaiting = FALSE;
    }
    return 0;
}
