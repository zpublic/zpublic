#ifndef __GAME_FG_BOARD_OBJECT_
#define __GAME_FG_BOARD_OBJECT_

#include "fg_card_def.h"

void Fight(
    const FGCard& p1Card,     ///> 玩家1出的牌
    const FGCard& p2Card,     ///> 玩家2出的牌
    int& p1Blood,               ///> 战斗后玩家1的血量变化 正数为加血 负数为掉血)
    int& p2Blood                ///> 战斗后玩家2的血量变化
    );

void GenerateCard(FGCard& card);

#endif