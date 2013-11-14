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
    FGCard retFGBoard;

    if (m_vecFGCard.size() < nIndex)
    {
        return retFGBoard;
    }

    retFGBoard = m_vecFGCard[nIndex];
    m_vecFGCard.erase(m_vecFGCard.begin() + nIndex);
    return retFGBoard;
}

void CFGPlayer::AddBoard(const FGCard& fgBoard)
{
    m_vecFGCard.push_back(fgBoard);
}

UINT CFGPlayer::GetBoardNum() const
{
    return m_vecFGCard.size();
}