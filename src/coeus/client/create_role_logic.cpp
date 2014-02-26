#include "stdafx.h"
#include "create_role_logic.h"

CreateRoleLogic::CreateRoleLogic()
{
    m_hLoginDlg = NULL;
}

void CreateRoleLogic::SetCreateRoleDlg( HWND hWnd )
{
    m_hLoginDlg = hWnd;
}

void CreateRoleLogic::SendCreateRole(const CString& strNickname, const uint8& gender, const uint8& belief, const uint8& characterType)
{
	CSCreateRoleReq cr_req;
	
	cr_req.nickname = CW2A(strNickname).m_psz;
	cr_req.gender = gender;
	cr_req.belief = belief;
	cr_req.character_type = characterType;
	
    NET.SendMessage(Opcodes::CSCreateRoleReq, cr_req);
}
void CreateRoleLogic::SendRandNickName(const uint8& gender, const uint8& type)
{
	CSGetRandomNameReq grn_req;

	grn_req.gender = gender;
	grn_req.type = type;
    NET.SendMessage(Opcodes::CSGetRandomNameReq, grn_req);
}
void CreateRoleLogic::SendCheckNickName(const CString& strNickname)
{
	CSCheckNicknameExistReq cn_req;
    CStringA csUserNameA = CW2A(strNickname).m_psz;

	cn_req.nickname = csUserNameA;
    NET.SendMessage(Opcodes::CSCheckNicknameExistReq, cn_req);
}

void CreateRoleLogic::NotifyRandNickNameSync(std::string const * nick)
{
	::SendMessage(m_hLoginDlg, msg_get_rand_name_result, 1, (LPARAM)nick);
}

void CreateRoleLogic::NotifyCheckNickNameSync(uint8 nErr)
{
	::SendMessage(m_hLoginDlg, msg_check_nickname_result, 1, (LPARAM)nErr);
}
void CreateRoleLogic::NotifyCheckNickName()
{
    ::PostMessage(m_hLoginDlg, msg_check_nickname_result, 0, 0);
}

void CreateRoleLogic::NotifyCreateRoleResultSync(uint8 nErr)
{
	::SendMessage(m_hLoginDlg, msg_create_role_result, 2, (LPARAM)nErr);
}
void CreateRoleLogic::NotifyCreateRoleResult()
{
    ::PostMessage(m_hLoginDlg, msg_create_role_result, 1, 0);
}
