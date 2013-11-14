#include "stdafx.h"
#include "fg_game.h"
#include "fg_card.h"
#include <ctime>

CFGGame::CFGGame()
{
}

CFGGame::~CFGGame()
{
}

BOOL CFGGame::_DealCard(UINT nNum)
{
    FGCard card;

    for (UINT n = 0; n < nNum; ++n)
    {
        GenerateCard(card);
        m_PlayerSeft.AddCard(card);
    }

    return TRUE;
}

BOOL CFGGame::Initialize()
{
    srand((unsigned int)time(NULL));
    _DealCard(8);
    m_PlayerSeft.AddLift(10);
    m_PlayerComputer.AddLift(10);
    return TRUE;
}

BOOL CFGGame::Unitialize()
{
    return TRUE;
}

BOOL CFGGame::OutCard(UINT nIndex)
{
    BOOL bReturn = FALSE;

    int iSelfLife = 0;
    int iComputerLift = 0;

    FGCard outBoard;
    FGCard outBoardComputer;

    if (!m_PlayerSeft.OutCard(nIndex, outBoard))
    {
        goto Exit;
    }

    GenerateCard(outBoardComputer);
    Fight(outBoard, outBoardComputer, iSelfLife, iComputerLift);
    m_PlayerSeft.AddLift(iSelfLife);
    m_PlayerComputer.AddLift(iComputerLift);

    bReturn = TRUE;
Exit:

    return bReturn;
}
