#ifndef __GAME_FG_BOARD_GAME
#define __GAME_FG_BOARD_GAME

#include "fg_game_def.h"

class CFGPlayer;
class CFGGame
{
public:
    CFGGame();

    ~CFGGame();

    BOOL Push(CFGPlayer* fgPlayer);

    BOOL Pop(UINT nIndex);

    BOOL DealBoard(UINT nIndex);

private:
    vecFGPlayerGame m_vecPlayerGame;                ///> мФ╪рап╠М
};

#endif