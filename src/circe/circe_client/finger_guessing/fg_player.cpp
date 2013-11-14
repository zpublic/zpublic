#include "stdafx.h"
#include "fg_player.h"

CFGPlayer::CFGPlayer()
{
}

CFGPlayer::~CFGPlayer()
{
}

void CFGPlayer::SetPlayerGame(const std::wstring& strName)
{
    m_strPlayerName = strName;
}

const std::wstring& CFGPlayer::GetPlayerGame() const
{
    return m_strPlayerName;
}

BOOL CFGPlayer::OutCard(UINT nIndex, FGCard& card)
{
    BOOL bReturn = FALSE;

    if (m_vecCard.size() < nIndex)
    {
        goto Exit;
    }

    card = m_vecCard[nIndex];
    m_vecCard.erase(m_vecCard.begin() + nIndex);

    bReturn = TRUE;
Exit:

    return bReturn;
}

void CFGPlayer::AddCard(const FGCard& fgCard)
{
    m_vecCard.push_back(fgCard);
}

UINT CFGPlayer::GetCardNum() const
{
    return m_vecCard.size();
}