#include "stdafx.h"
#include "fg_game.h"

CFGBoardGame::CFGBoardGame()
{
}

CFGBoardGame::~CFGBoardGame()
{
}

BOOL CFGBoardGame::Push(CFGPlayer* fgPlayer)
{
    return TRUE;
}

BOOL CFGBoardGame::Pop(UINT nIndex)
{
    return TRUE;
}

BOOL CFGBoardGame::DealBoard(UINT nIndex)
{
    BOOL bReturn = FALSE;
    ShuffleRule shrule;
    vecFGBoard vecBoardList;

    if (m_vecPlayerGame.size() < nIndex)
    {
        goto Exit;
    }

    CFGPlayer* pPlayer = m_vecPlayerGame[nIndex];

    if (!pPlayer)
    {
        goto Exit;
    }

    shrule.Shuffle(vecBoardList, 1);

    for (vecFGBoardConstIt it = vecBoardList.begin();
        it != vecBoardList.end();
        ++it)
    {
        pPlayer->AddBoard(*it);
    }

    bReturn = TRUE;
Exit:
    return bReturn;
}