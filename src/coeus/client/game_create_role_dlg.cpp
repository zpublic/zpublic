#include "stdafx.h"
#include "game_create_role_dlg.h"
#include "window_manager.h"

#define WINDOW_MAX_WIDTH            460
#define WINDOW_MAX_HEIGHT           465

#define WINDOW_MIN_WIDTH            460
#define WINDOW_MIN_HEIGHT           55

#define TIMER_BUTTON_MOVE           1

#define NUM_BUFFER_MAX              260

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
    EndDialog(GWIN_ID_EXIT);
}

void GameCreateRoleDlg::OnSysCommand(UINT nID, CPoint point)
{
    SetMsgHandled(FALSE);

    if (nID == SC_CLOSE)
    {
        EndDialog(GWIN_ID_EXIT);
    }
}

void GameCreateRoleDlg::OnTimer(UINT_PTR nIDEvent)
{
}

void GameCreateRoleDlg::OnBtnClose()
{
    EndDialog(GWIN_ID_EXIT);
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
	if(exists) {
		::MessageBox(m_hWnd, L"��ɫ�Ѿ����ڣ������ԡ�", L"coeus", 0);
	} else {
		::MessageBox(m_hWnd, L"��ɫû�б�ע�ᡣ", L"coeus", 0);
	}
    return TRUE;
}

void GameCreateRoleDlg::OnSelectHuntsmanMan()
{
	m_bSelectedGender = GENDER_MALE;
    SelectRole(ID_SELECT_HUNTSMAN_MAN_BTN, ID_ROLE_IMG, "img_role_huntsman_man");
    SetItemText(ID_ROLE_NAME, L"��������");
    SetItemText(ID_ROLE_EXPLAIN, L"���������������ռ���Ȼ���и����ܷ����ʯͷ");
    SetItemVisible(ID_ROLE_EXPLAIN_DLG, TRUE);
}

void GameCreateRoleDlg::OnSelectHuntsmanWoMan()
{
	m_bSelectedGender = GENDER_FEMALE;
    SelectRole(ID_SELECT_HUNTSMAN_WOMAN_BTN, ID_ROLE_IMG, "img_role_huntsman_woman");
    SetItemText(ID_ROLE_NAME, L"��������");
    SetItemText(ID_ROLE_EXPLAIN, L"���������������ռ���Ȼ���и����ܷ����ʯͷ");
    SetItemVisible(ID_ROLE_EXPLAIN_DLG, TRUE);
}

void GameCreateRoleDlg::OnSelectMechanicianMan()
{
	m_bSelectedGender = GENDER_MALE;
    SelectRole(ID_SELECT_MECHANICIAN_MAN_BTN, ID_ROLE_IMG, "img_role_mechanician_man");
    SetItemText(ID_ROLE_NAME, L"��еʦ");
    SetItemText(ID_ROLE_EXPLAIN, L"���ȵķ����ң���е���찮���ߣ����Ƚ��ļ����ͳ��ߵ���������������");
    SetItemVisible(ID_ROLE_EXPLAIN_DLG, TRUE);
}

void GameCreateRoleDlg::OnSelectMechanicianWoMan()
{
	m_bSelectedGender = GENDER_FEMALE;
    SelectRole(ID_SELECT_MECHANICIAN_WOMAN_BTN, ID_ROLE_IMG, "img_role_mechanician_woman");
    SetItemText(ID_ROLE_NAME, L"��еʦ");
    SetItemText(ID_ROLE_EXPLAIN, L"���ȵķ����ң���е���찮���ߣ����Ƚ��ļ����ͳ��ߵ���������������");
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
    EndDialog(GWIN_ID_MAIN);
}
