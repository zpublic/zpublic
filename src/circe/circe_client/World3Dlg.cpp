#include "stdafx.h"
#include "resource.h"
#include "game_handler.h"
#include "World3Dlg.h"

CWorld3Dlg::CWorld3Dlg()
{
    m_iSeftLift = 0;
    m_iComputey = 0;
}

LRESULT CWorld3Dlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());
    m_SeftCardListBox.Attach(GetDlgItem(IDC_LIST1));
    m_FightInfoEdit.Attach(GetDlgItem(IDC_EDIT2));

    GameHandler::fightcard.SetGameDlg(m_hWnd);
    GameHandler::fightcard.InitializeGame(3, 10);
    return TRUE;
}

LRESULT CWorld3Dlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    EndDialog(0);
    return 0;
}

LRESULT CWorld3Dlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    GameHandler::fightcard.OutCard(m_SeftCardListBox.GetCurSel());
    return 0;
}

void CWorld3Dlg::_ConvertCardToStirng(const FGCard& card, CString& csConvertString)
{
    CString cardType;
    switch (card.emType)
    {
    case emCardType_Scissor:
        cardType = L"剪刀";
        break;
    case emCardType_Stone:
        cardType = L"石头";
        break;
    case emCardType_Cloth:
        cardType = L"布    ";
        break;
    default:
        break;
    }

    csConvertString.Format(L"%s  ", cardType);
	if (0 != card.byRevert)
	{
	  csConvertString.AppendFormat(L"回复值:%d", card.byRevert);
	}
	if (0 != card.bySuckBlood)
	{
	  csConvertString.AppendFormat(L" 吸血值:%d", card.bySuckBlood);
	}
	if (0 != card.dwSpecial)
	{
	  csConvertString.AppendFormat(L" 免伤:%d", card.dwSpecial);
	}
	if (0 != card.byPressAttack)
	{
	  csConvertString.AppendFormat(L" 强袭:%d", card.byPressAttack);
	}
	if (0 != card.byCriticalStrike)
	{
	  csConvertString.AppendFormat(L" 暴击:%d", card.byCriticalStrike);
	}
}

void CWorld3Dlg::_ConvertDamageToStirng(int iDamageValue, CString& csConvertString)
{
    if (iDamageValue > 0)
    {
        csConvertString = L"  恢复生命值:";
    }
    else
    {
        csConvertString = L"  战斗损失生命值:";
    }
    
    csConvertString.AppendFormat(L" %d", iDamageValue);
}

bool CWorld3Dlg::_PrintFightInfo(const FGFightInfo* pInfo)
{
    if (!pInfo)
    {
        return false;
    }

    CString csFightInfo;
    CString csSelfLiftText;
    CString csComputeLiftText;
    CString csComputeyCard;
    CString csSelfCard;

    m_FightInfoEdit.GetWindowText(csFightInfo);

    if (!csFightInfo.IsEmpty())
    {
        csFightInfo += L"\r\n";
    }

    _ConvertCardToStirng(pInfo->outComputeCard, csComputeyCard);
    _ConvertCardToStirng(pInfo->outSelfCard, csSelfCard);
    _ConvertDamageToStirng(pInfo->iComputeDamage, csComputeLiftText);
    _ConvertDamageToStirng(pInfo->iSeftDamage, csSelfLiftText);

    csFightInfo.AppendFormat(L"对方打出: %s, %s \r\n    你打出: %s, %s ",
        csComputeyCard,
        csComputeLiftText,
        csSelfCard,
        csSelfLiftText,
        m_Game.GetSelfDamageLift());

    m_FightInfoEdit.SetWindowText(csFightInfo);

    return true;
}

LRESULT CWorld3Dlg::OnGameUpdate(UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    BOOL& bHandled)
{
    VECFGCARD* pSeftcardUpdate = (VECFGCARD*)wParam;
    FGGameInfo* pGameInfo = (FGGameInfo*)lParam;

    if (!pSeftcardUpdate)
    {
        return FALSE;
    }

    CString csCard;
    CString csSeftLiftValue;
    CString csComputerLiftValue;

    for (UINT n = 0; n < pSeftcardUpdate->size(); ++n)
    {
        _ConvertCardToStirng((*pSeftcardUpdate)[n], csCard);
        m_SeftCardListBox.AddString(csCard);
    }

    csSeftLiftValue.Format(L"%d", pGameInfo->iSeftLift);
    csComputerLiftValue.Format(L"%d", pGameInfo->iComputeLift);

    SetDlgItemText(IDC_STATIC7, csSeftLiftValue);
    SetDlgItemText(IDC_STATIC6, csComputerLiftValue);
    return TRUE;
}

LRESULT CWorld3Dlg::OnGameFight(UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    BOOL& bHandled)
{
    VECFGCARD* pSeftcardUpdate = (VECFGCARD*)wParam;
    FGFightInfo* pFightInfo = (FGFightInfo*)lParam;

    if (!pSeftcardUpdate
        || !pFightInfo)
    {
        return FALSE;
    }

    CString csPutCard;
    CString csSeftLiftValue;
    CString csComputerLiftValue;

    _ConvertCardToStirng((*pSeftcardUpdate)[pFightInfo->iSelfOutCard], csPutCard);
    m_SeftCardListBox.DeleteString(pFightInfo->iSelfOutCard);
    m_SeftCardListBox.AddString(csPutCard);
    _PrintFightInfo(pFightInfo);

    csSeftLiftValue.Format(L"%d", pFightInfo->GameInfo.iSeftLift);
    csComputerLiftValue.Format(L"%d", pFightInfo->GameInfo.iComputeLift);

    SetDlgItemText(IDC_STATIC7, csSeftLiftValue);
    SetDlgItemText(IDC_STATIC6, csComputerLiftValue);
    return TRUE;
}