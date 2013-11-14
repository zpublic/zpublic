#include "stdafx.h"
#include "fg_player_base.h"

FGPlayerBase::FGPlayerBase()
    : m_nLift()
{
}

FGPlayerBase::~FGPlayerBase()
{
}

void FGPlayerBase::AddLift(UINT nLift)
{
    m_nLift += nLift;
}

void FGPlayerBase::SubLift(UINT nLift)
{
    m_nLift -= nLift;
}