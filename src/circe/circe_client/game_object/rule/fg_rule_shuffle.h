#ifndef __GAME_FG_RULE_BOARD_OBJECT
#define __GAME_FG_RULE_BOARD_OBJECT

#include "irule_board.h"
#include "game_object/board/fg_board.h"
#include <ctime>
#include <cstdlib>

struct ShuffleRule
{
    BOOL Shuffle(vecFGBoard& boardlist, UINT nNum)
    {
        FGBoard board;
        srand(unsigned(time(NULL)));

        for (UINT nIndex = 0;
            nIndex < nNum;
            ++nIndex)
        {
            board.Reset();
            board.byRevert = (rand() % 1) + 1;
            board.bySuckBlood = (rand() % 2) + 1;
            board.dwSpecial = (rand() % 1) + 1;
            board.byPressAttack = (rand() % 1) + 1;
            board.byCriticalStrike = (rand() % 1) + 1;
            board.byCriticalStrike = (rand() % emBoardType_Cloth) + 1;
            boardlist.push_back(board);
        }

        return TRUE;
    }
};

#endif
