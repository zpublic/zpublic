#include "stdafx.h"
#include "fg_game.h"

CFGGame::CFGGame()
{
}

CFGGame::~CFGGame()
{
}

BOOL CFGGame::Push(CFGPlayer* fgPlayer)
{
    return TRUE;
}

BOOL CFGGame::Pop(UINT nIndex)
{
    return TRUE;
}

BOOL CFGGame::DealBoard(UINT nIndex)
{
    BOOL bReturn = FALSE;

    if (m_vecPlayerGame.size() < nIndex)
    {
        goto Exit;
    }

    CFGPlayer* pPlayer = m_vecPlayerGame[nIndex];

    if (!pPlayer)
    {
        goto Exit;
    }

    bReturn = TRUE;
Exit:
    return bReturn;
}