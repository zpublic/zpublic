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

FGBoard CFGPlayer::OutBoard(UINT nIndex)
{
    FGBoard retFGBoard;

    if (m_vecFGBorad.size() < nIndex)
    {
        return retFGBoard;
    }

    retFGBoard = m_vecFGBorad[nIndex];
    m_vecFGBorad.erase(m_vecFGBorad.begin() + nIndex);
    return retFGBoard;
}

void CFGPlayer::AddBoard(const FGBoard& fgBoard)
{
    m_vecFGBorad.push_back(fgBoard);
}

UINT CFGPlayer::GetBoardNum() const
{
    return m_vecFGBorad.size();
}