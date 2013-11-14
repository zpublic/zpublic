#ifndef __GAME_FG_BOARD_OBJECT_
#define __GAME_FG_BOARD_OBJECT_

#include "fg_iboard.h"

/*
    dwSpecial 为特殊附加字段
    免伤            即使回合失败，也不会掉血
*/

struct CFGBoard
{
    CFGBoard()
    {
        byRevert = 0;
        bySuckBlood = 0;
        dwSpecial = 0;
        byPressAttack = 0;
        byCriticalStrike = 0;
        emBoardType = emBoardType_Null;
    }

    emBoardType emBoardType;    ///> 牌类型
    BYTE byRevert;              ///> 回复
    BYTE bySuckBlood;           ///> 吸血
    DWORD dwSpecial;            ///> 特殊附加
    BYTE byPressAttack;         ///> 强袭
    BYTE byCriticalStrike;      ///> 暴击
};

#endif