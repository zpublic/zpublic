#ifndef __GAME_FG_BOARD_GAME
#define __GAME_FG_BOARD_GAME

#include "fg_player.h"

class CFGGame
{
public:
    CFGGame();

    ~CFGGame();

    BOOL Initialize();

    BOOL Unitialize();

    BOOL OutCard(UINT nIndex);

    BOOL GetCardList(VECFGCARD& vecCard);

    int GetSelfLift() const;

    int GetComputeyLift() const;

    int GetSelfDamageLift() const;

    int GetComputeDamageLift() const;

    const FGCard& GetOutComputeyCard() const;

    const FGCard& GetOutSelfCard() const;

private:
    BOOL _DealCard(UINT nNum);

private:
    CFGPlayer m_PlayerSelf;                         ///> 自己
    CFGPlayer m_PlayerComputer;                     ///> 电脑
    int m_iSelfDamageLift;                          ///> 战损
    int m_iComputeDamageLift;                       ///> 战损
    FGCard m_OutComputeyCard;                       ///> 电脑的出牌
    FGCard m_OutSelfCard;                           ///> 自己的出牌
};

#endif