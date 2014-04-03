#include "stdafx.h"
#include "character_info.h"

CharacterInfo::CharacterInfo()
{
    m_hDlg = NULL;
}

void CharacterInfo::SetCallbackDlg( HWND hWnd )
{
    m_hDlg = hWnd;
}

void CharacterInfo::PropertiesReq()
{
    CSPropertiesReq propertiesRequest;
    NET.SendMessage(Opcodes::CSPropertiesReq, propertiesRequest);
}

void CharacterInfo::NotifyProperties( uint64 coins, uint32 exp, uint32 level )
{
    Properties p;
    p.coins = coins;
    p.exp = exp;
    p.level = level;
    ::SendMessage(m_hDlg, msg_character_info, (WPARAM)&p, 0);
}

