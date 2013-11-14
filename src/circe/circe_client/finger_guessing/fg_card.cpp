#include "stdafx.h"
#include "fg_card.h"

void _FightCalculate(
    const FGCard& WinnerCard,
    const FGCard& LoserCard,
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

void Fight(
    const FGCard& p1Card, /*/> 玩家1出的牌 */
    const FGCard& p2Card, /*/> 玩家2出的牌 */
    int& p1Blood, /*/> 战?后玩家1的血量变 ?正数为加血 负数为掉血) */
    int& p2Blood /*/> 战?后玩家2的血量变 ?*/ )
{
    assert(p1Card.emType != emCardType_Null);
    assert(p2Card.emType != emCardType_Null);

    ///> 回复属性是一定加血的
    p1Blood = p1Card.byRevert;
    p2Blood = p2Card.byRevert;

    ///> 平局
    if (p1Card.emType == p2Card.emType)
    {
        return;
    }

    if (   (p1Card.emType == emCardType_Cloth && p2Card.emType == emCardType_Stone)
        || (p1Card.emType == emCardType_Stone && p2Card.emType == emCardType_Scissor)
        || (p1Card.emType == emCardType_Scissor && p2Card.emType == emCardType_Cloth) )
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

void GenerateCard( FGCard& card )
{
    card.Reset();

    switch (rand() % 3)
    {
    case 0:
        card.emType = emCardType_Cloth;
        break;
    case 1:
        card.emType = emCardType_Scissor;
        break;
    case 2:
        card.emType = emCardType_Stone;
        break;
    }

    ///> 10%几率免伤
    if (rand() % 10 == 0)
        card.dwSpecial &= 1;

    ///> 10%几率回复1
    if (rand() % 10 == 0)
        card.byRevert = 1;

    ///> 强袭几率 0 1 2 为 50% 30% 20%
    int n = rand() % 10;
    if (n < 2)
        card.byPressAttack = 2;
    else if (n < 5)
        card.byPressAttack = 1;

    ///> 吸血0 1 2几率分别 80 10 10
    n = rand() % 10;
    if (n < 1)
        card.bySuckBlood = 1;
    else if (n < 2)
        card.bySuckBlood = 2;

    ///> 0 1 2 3 属性的几率分别为 50% 20% 20% 10%
    n = rand() % 10;
    if (n < 1)
        card.byCriticalStrike = 3;
    else if (n < 3)
        card.byCriticalStrike = 2;
    else if (n < 5)
        card.byCriticalStrike = 1;
}
