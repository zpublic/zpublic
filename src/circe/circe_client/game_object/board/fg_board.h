#ifndef __GAME_FG_BOARD_OBJECT_
#define __GAME_FG_BOARD_OBJECT_

#include "fg_iboard.h"

/*
    dwSpecial 为特殊附加字段
    免伤            即使回合失败，也不会掉血
*/

struct FGBoard
{
    FGBoard()
    {
        Reset();
    }

    void Reset()
    {
        byRevert = 0;
        bySuckBlood = 0;
        dwSpecial = 0;
        byPressAttack = 0;
        byCriticalStrike = 0;
        emBoardType = emBoardType_Null;
    }

public:
    void Fight(
        const FGBoard& p1Card,     ///> 玩家1出的牌
        const FGBoard& p2Card,     ///> 玩家2出的牌
        int& p1Blood,               ///> 战斗后玩家1的血量变化 正数为加血 负数为掉血)
        int& p2Blood                ///> 战斗后玩家2的血量变化
        )
    {
        assert(p1Card.emBoardType != emBoardType_Null);
        assert(p2Card.emBoardType != emBoardType_Null);

        ///> 回复属性是一定加血的
        p1Blood = p1Card.byRevert;
        p2Blood = p2Card.byRevert;

        ///> 平局
        if (p1Card.emBoardType == p2Card.emBoardType)
        {
            return;
        }

        if (   (p1Card.emBoardType == emBoardType_Cloth && p2Card.emBoardType == emBoardType_Stone)
            || (p1Card.emBoardType == emBoardType_Stone && p2Card.emBoardType == emBoardType_Scissor)
            || (p1Card.emBoardType == emBoardType_Scissor && p2Card.emBoardType == emBoardType_Cloth) )
        {
            ///> 玩家1胜利
            _FightCalculate(p1Card, p2Card, p1Blood, p2Blood);
        }
        else
        {
            ///> 玩家2胜利
            _FightCalculate(p2Card, p1Card, p2Blood, p1Blood);
        }
    }

    void GenerateCard(FGBoard& card)
    {
        card.Reset();

        switch (rand() % 3)
        {
        case 0:
            card.emBoardType = emBoardType_Cloth;
            break;
        case 1:
            card.emBoardType = emBoardType_Scissor;
            break;
        case 2:
            card.emBoardType = emBoardType_Stone;
            break;
        }

        ///> 10%几率免伤
        if (rand() % 10 == 0)
            card.dwSpecial &= 1;

        ///> 0 1 2 属性的几率分别为 50% 30% 20%
        int n = rand() % 10;
        if (n < 2)
            card.byRevert = 2;
        else if (n < 5)
            card.byRevert = 1;
        n = rand() % 10;
        if (n < 2)
            card.byPressAttack = 2;
        else if (n < 5)
            card.byPressAttack = 1;

        ///> 0 1 2 3 属性的几率分别为 50% 20% 20% 10%
        n = rand() % 10;
        if (n < 1)
            card.bySuckBlood = 3;
        else if (n < 3)
            card.bySuckBlood = 2;
        else if (n < 5)
            card.bySuckBlood = 1;
        n = rand() % 10;
        if (n < 1)
            card.byCriticalStrike = 3;
        else if (n < 3)
            card.byCriticalStrike = 2;
        else if (n < 5)
            card.byCriticalStrike = 1;
    }

private:
    void _FightCalculate(
        const FGBoard& WinnerCard,
        const FGBoard& LoserCard,
        int& WinnerBlood,
        int& LoserBlood )
    {
        if ((LoserCard.dwSpecial & 1) != 0)
        {
            ///> 输家免伤
            return;
        }
        else
        {
            ///> 赢家吸血效果
            WinnerBlood += WinnerCard.bySuckBlood;
            ///> 输家-1血 外加强袭效果
            LoserBlood -= 1 + WinnerCard.byPressAttack;
            ///> 50%几率暴击效果
            if (WinnerCard.byCriticalStrike && (rand() % 2) == 0)
            {
                LoserBlood -= WinnerCard.byCriticalStrike;
            }
        }
    }

    emBoardType emBoardType;    ///> 牌类型
    BYTE byRevert;              ///> 回复
    BYTE bySuckBlood;           ///> 吸血
    DWORD dwSpecial;            ///> 特殊附加
    BYTE byPressAttack;         ///> 强袭
    BYTE byCriticalStrike;      ///> 暴击
};

#endif