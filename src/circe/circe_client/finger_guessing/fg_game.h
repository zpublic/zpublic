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

    int GetSeftLift() const;

    int GetComputeyLift() const;

    int GetBattleDamageLift() const;

    const FGCard& GetOutComputeyCard() const;

private:
    BOOL _DealCard(UINT nNum);

private:
    CFGPlayer m_PlayerSeft;                         ///> 自己
    CFGPlayer m_PlayerComputer;                     ///> 电脑
    int m_iBattleDamageLift;                        ///> 战损
    FGCard m_OutComputeyCard;                       ///> 电脑的出牌
};

#endif