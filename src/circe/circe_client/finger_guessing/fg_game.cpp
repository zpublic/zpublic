#include "stdafx.h"
#include "fg_game.h"
#include "fg_card.h"
#include "fg_xml_parse.h"
#include <ctime>

CFGGame::CFGGame()
{
    m_iBattleDamageLift = 0;
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
    CFGXMLParse parsexml;
    VECFGCARD vecCard;
    srand((unsigned int)time(NULL));
    _DealCard(8);
    m_PlayerSeft.AddLift(10);
    m_PlayerComputer.AddLift(10);

    wchar_t filePath[MAX_PATH] = {0};
    ::GetModuleFileNameW(0, filePath, MAX_PATH);
    ::PathRemoveFileSpecW(filePath);
    ::PathAppendW(filePath, _T("cards.xml"));
    parsexml.Parse(filePath, vecCard);
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

    if (!m_PlayerSeft.OutCard(nIndex, outCard))
    {
        goto Exit;
    }

    GenerateCard(outCardComputer);
    GenerateCard(dealCard);
    Fight(outCard, outCardComputer, iSelfLife, iComputerLift);
    m_PlayerSeft.AddLift(iSelfLife);
    m_PlayerSeft.AddCard(dealCard);
    m_PlayerComputer.AddLift(iComputerLift);
    m_OutComputeyCard = outCardComputer;
    m_iBattleDamageLift = iSelfLife;
    bReturn = TRUE;
Exit:

    return bReturn;
}

BOOL CFGGame::GetCardList(VECFGCARD& vecCard)
{
    BOOL bReturn = FALSE;
    FGCard card;

    for (UINT n = 0; n < m_PlayerSeft.GetCardNum(); ++n)
    {
        if (!m_PlayerSeft.GetCardInfo(n, card))
        {
            goto Exit;
        }

        vecCard.push_back(card);
    }

    bReturn = TRUE;
Exit:

    return bReturn;
}

int CFGGame::GetSeftLift() const
{
    return m_PlayerSeft.GetLift();
}

const FGCard& CFGGame::GetOutComputeyCard() const
{
    return m_OutComputeyCard;
}

int CFGGame::GetBattleDamageLift() const
{
    return m_iBattleDamageLift;
}

int CFGGame::GetComputeyLift() const
{
    return m_PlayerComputer.GetLift();
}
