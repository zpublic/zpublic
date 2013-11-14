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

FGCard CFGPlayer::OutBoard(UINT nIndex)
{
    FGCard retCard;

    if (m_vecCard.size() < nIndex)
    {
        return retCard;
    }

    retCard = m_vecCard[nIndex];
    m_vecCard.erase(m_vecCard.begin() + nIndex);
    return retCard;
}

void CFGPlayer::AddBoard(const FGCard& fgCard)
{
    m_vecCard.push_back(fgCard);
}

UINT CFGPlayer::GetBoardNum() const
{
    return m_vecCard.size();
}