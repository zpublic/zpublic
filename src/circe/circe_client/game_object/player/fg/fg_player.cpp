#include "stdafx.h"
#include "fg_player.h"

CFGPlayerGame::CFGPlayerGame()
{
}

CFGPlayerGame::~CFGPlayerGame()
{
}

void CFGPlayerGame::SetPlayerGame(const std::wstring& strName)
{
    m_strPlayerName = strName;
}

const std::wstring& CFGPlayerGame::GetPlayerGame() const
{
    return m_strPlayerName;
}

FGBoard CFGPlayerGame::OutBoard(UINT nIndex)
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

void CFGPlayerGame::AddBoard(const FGBoard& fgBoard)
{
    m_vecFGBorad.push_back(fgBoard);
}

UINT CFGPlayerGame::GetBoardNum() const
{
    return m_vecFGBorad.size();
}