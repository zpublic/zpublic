#ifndef __GAME_FG_BOARD_OBJECT_DEF
#define __GAME_FG_BOARD_OBJECT_DEF

typedef enum _emCardType
{
    emCardType_Null,
    emCardType_Stone,
    emCardType_Scissor,
    emCardType_Cloth,
}emCardType;

#include <vector>
#include <string>

/*
    dwSpecial 为特殊附加字段
    免伤            即使回合失败，也不会掉血
*/
struct FGCard
{
    FGCard()
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
        emType = emCardType_Null;
    }

    emCardType emType;    ///> 牌类型
    BYTE byRevert;              ///> 回复
    BYTE bySuckBlood;           ///> 吸血
    DWORD dwSpecial;            ///> 特殊附加
    BYTE byPressAttack;         ///> 强袭
    BYTE byCriticalStrike;      ///> 暴击
};

typedef std::vector<FGCard> vecFGCard;
typedef std::vector<FGCard>::iterator vecFGCardIt;
typedef std::vector<FGCard>::const_iterator vecFGCardConstIt;
#endif