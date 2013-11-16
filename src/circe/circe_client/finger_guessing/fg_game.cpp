#include "stdafx.h"
#include "fg_game.h"
#include "fg_card.h"
#include "fg_card_info.h"
#include <ctime>

CFGGame::CFGGame()
{
    m_iSelfDamageLift = 0;
    m_iComputeDamageLift = 0;
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
        m_PlayerSelf.AddCard(card);
    }

    return TRUE;
}

BOOL CFGGame::Initialize()
{
    CFGCardInfo parsexml;
    VECFGCARD vecCard;
    srand((unsigned int)time(NULL));
    _DealCard(8);
    m_PlayerSelf.AddLift(10);
    m_PlayerComputer.AddLift(10);

    parsexml.Parse(L".\\data\\cards.xml", vecCard);
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

    FGCard outCard;
    FGCard outCardComputer;
    FGCard dealCard;

    if (!m_PlayerSelf.OutCard(nIndex, outCard))
    {
        goto Exit;
    }

    GenerateCard(outCardComputer);
    GenerateCard(dealCard);

    Fight(outCard, outCardComputer, iSelfLife, iComputerLift);

    m_PlayerSelf.AddLift(iSelfLife);
    m_PlayerComputer.AddLift(iComputerLift);

    m_PlayerSelf.AddCard(dealCard);

    m_OutComputeyCard = outCardComputer;
    m_OutSelfCard = outCard;
    m_iSelfDamageLift = iSelfLife;
    m_iComputeDamageLift = iComputerLift;

    bReturn = TRUE;
Exit:

    return bReturn;
}

BOOL CFGGame::GetCardList(VECFGCARD& vecCard)
{
    BOOL bReturn = FALSE;
    FGCard card;

    for (UINT n = 0; n < m_PlayerSelf.GetCardNum(); ++n)
    {
        if (!m_PlayerSelf.GetCardInfo(n, card))
        {
            goto Exit;
        }

        vecCard.push_back(card);
    }

    bReturn = TRUE;
Exit:

    return bReturn;
}

int CFGGame::GetSelfLift() const
{
    return m_PlayerSelf.GetLift();
}

const FGCard& CFGGame::GetOutComputeyCard() const
{
    return m_OutComputeyCard;
}

int CFGGame::GetSelfDamageLift() const
{
    return m_iSelfDamageLift;
}

int CFGGame::GetComputeyLift() const
{
    return m_PlayerComputer.GetLift();
}

int CFGGame::GetComputeDamageLift() const
{
    return m_iComputeDamageLift;
}

const FGCard& CFGGame::GetOutSelfCard() const
{
    return m_OutSelfCard;
}
