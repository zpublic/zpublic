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

private:
    BOOL _DealCard(UINT nNum);

private:
    CFGPlayer m_PlayerSeft;                         ///> вт╪╨
    CFGPlayer m_PlayerComputer;                     ///> ╣Гдт
};

#endif