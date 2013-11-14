#include "stdafx.h"
#include "player_game.h"

CPlayerGameBase::CPlayerGameBase()
    : m_nLift()
{
}

CPlayerGameBase::~CPlayerGameBase()
{
}

void CPlayerGameBase::AddLift(UINT nLift)
{
    m_nLift += nLift;
}

void CPlayerGameBase::SubLift(UINT nLift)
{
    m_nLift -= nLift;
}