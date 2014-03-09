#include "stdafx.h"
#include "game_create_role_dlg.h"
#include "role_config.h"

LRESULT GameCreateRoleDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
    m_NickNameEdit.Create( 
        GetViewHWND(), NULL, NULL, 
        WS_CHILD, 
        0, 4000, NULL);
    m_NickNameEdit.SetBgColor(RGB(255,255,255));
    m_NickNameEdit.SetColor(RGB(0,0,0));
    m_NickNameEdit.SetNotify(m_hWnd);

    OnSelectHuntsmanMan();
    return TRUE;
}

void GameCreateRoleDlg::OnClose()
{
    EndDialog(IDCLOSE);
}

void GameCreateRoleDlg::OnSysCommand(UINT nID, CPoint point)
{
    SetMsgHandled(FALSE);

    if (nID == SC_CLOSE)
    {
        EndDialog(IDCLOSE);
    }
}

void GameCreateRoleDlg::OnTimer(UINT_PTR nIDEvent)
{
}

void GameCreateRoleDlg::OnBtnClose()
{
    EndDialog(IDCLOSE);
}

LRESULT GameCreateRoleDlg::OnRoleResult(UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    BOOL& bHandled)
{
    return TRUE;
}

LRESULT GameCreateRoleDlg::OnRandResult(UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    BOOL& bHandled)
{
    std::string const * nick = (std::string const *)lParam;
    CString cstr(nick->c_str());
    m_NickNameEdit.SetText(cstr);
    return TRUE;
}

LRESULT GameCreateRoleDlg::OnCheckResult(UINT uMsg,
    WPARAM exists,
    LPARAM lParam,
    BOOL& bHandled)
{
    if(exists)
    {
        ::MessageBox(m_hWnd, L"角色已经存在，请重试。", L"coeus", 0);
    }
    else
    {
        ::MessageBox(m_hWnd, L"角色没有被注册。", L"coeus", 0);
    }

    return TRUE;
}

void GameCreateRoleDlg::OnSelectHuntsmanMan()
{
    CString csName;
    CString csDecs;
    CStringA csImgSkin;
    RoleConfig::get_mutable_instance().gainRoleConfigInfo(0, 0, csName, csDecs, csImgSkin);

    m_bSelectedGender = GENDER_MALE;
    SelectRole(ID_SELECT_HUNTSMAN_MAN_BTN, ID_ROLE_IMG, csImgSkin);
    SetItemText(ID_ROLE_NAME, csName);
    SetItemText(ID_ROLE_EXPLAIN, csDecs);
    SetItemVisible(ID_ROLE_EXPLAIN_DLG, TRUE);
}

void GameCreateRoleDlg::OnSelectHuntsmanWoMan()
{
    CString csName;
    CString csDecs;
    CStringA csImgSkin;
    RoleConfig::get_mutable_instance().gainRoleConfigInfo(0, 1, csName, csDecs, csImgSkin);

    m_bSelectedGender = GENDER_FEMALE;
    SelectRole(ID_SELECT_HUNTSMAN_WOMAN_BTN, ID_ROLE_IMG, csImgSkin);
    SetItemText(ID_ROLE_NAME, csName);
    SetItemText(ID_ROLE_EXPLAIN, csDecs);
    SetItemVisible(ID_ROLE_EXPLAIN_DLG, TRUE);
}

void GameCreateRoleDlg::OnSelectMechanicianMan()
{
    CString csName;
    CString csDecs;
    CStringA csImgSkin;
    RoleConfig::get_mutable_instance().gainRoleConfigInfo(1, 0, csName, csDecs, csImgSkin);

    m_bSelectedGender = GENDER_MALE;
    SelectRole(ID_SELECT_MECHANICIAN_MAN_BTN, ID_ROLE_IMG, csImgSkin);
    SetItemText(ID_ROLE_NAME, csName);
    SetItemText(ID_ROLE_EXPLAIN, csDecs);
    SetItemVisible(ID_ROLE_EXPLAIN_DLG, TRUE);
}

void GameCreateRoleDlg::OnSelectMechanicianWoMan()
{
    CString csName;
    CString csDecs;
    CStringA csImgSkin;
    RoleConfig::get_mutable_instance().gainRoleConfigInfo(1, 1, csName, csDecs, csImgSkin);

    m_bSelectedGender = GENDER_FEMALE;
    SelectRole(ID_SELECT_MECHANICIAN_WOMAN_BTN, ID_ROLE_IMG, csImgSkin);
    SetItemText(ID_ROLE_NAME, csName);
    SetItemText(ID_ROLE_EXPLAIN, csDecs);
    SetItemVisible(ID_ROLE_EXPLAIN_DLG, TRUE);
}

void GameCreateRoleDlg::SelectRole(UINT nBtnId, UINT nImgId, const CStringA& csSkinName)
{
    SetItemAttribute(nBtnId, "sub", "4");
    SetItemAttribute(nImgId, "skin", csSkinName);
}

void GameCreateRoleDlg::OnBtnRandNickname()
{
    GameLogic::crole.SetCreateRoleDlg(m_hWnd);
    GameLogic::crole.SendRandNickName(m_bSelectedGender, 0);
}

void GameCreateRoleDlg::OnBtnCheckNickname()
{
    CString csNickname;
    m_NickNameEdit.GetText(csNickname);

    if (csNickname.IsEmpty())
    {
        m_NickNameEdit.SetFocus();
        return;
    }

    GameLogic::crole.SetCreateRoleDlg(m_hWnd);
    GameLogic::crole.SendCheckNickName(csNickname);
}

void GameCreateRoleDlg::OnBtnOK()
{
    EndDialog(IDOK);
}
