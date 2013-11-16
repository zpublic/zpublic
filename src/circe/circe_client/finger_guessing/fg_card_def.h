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
        nId = 0;
        byRevert = 0;
        bySuckBlood = 0;
        dwSpecial = 0;
        byPressAttack = 0;
        byCriticalStrike = 0;
        emType = emCardType_Null;
    }

    UINT nId;
    std::wstring strName;       ///> 牌名称
    std::wstring strDecs;       ///> 牌说明
    emCardType emType;          ///> 牌类型
    BYTE byRevert;              ///> 回复
    BYTE bySuckBlood;           ///> 吸血
    DWORD dwSpecial;            ///> 特殊附加
    BYTE byPressAttack;         ///> 强袭
    BYTE byCriticalStrike;      ///> 暴击
};

struct FGGameInfo
{
    FGGameInfo()
    {
        iSeftLift = 0;
        iComputeLift = 0;
    }

    int iSeftLift;              ///> 自己的血量
    int iComputeLift;           ///> 电脑的血量
};

struct FGFightInfo
{
    FGFightInfo()
    {
        iSeftDamage = 0;
        iComputeDamage = 0;
        iSelfOutCard = 0;
        iComputeOutCard = 0;
    }

    int iSelfOutCard;           ///> 自己出牌的索引
    int iComputeOutCard;        ///> 电脑出牌的索引
    int iSeftDamage;            ///> 自己战损
    int iComputeDamage;         ///> 电脑战损
    FGCard outSelfCard;         ///> 自己的出牌
    FGCard outComputeCard;      ///> 电脑的出牌
    FGGameInfo GameInfo;        ///> 游戏信息
};

typedef std::vector<FGCard> VECFGCARD;
typedef std::vector<FGCard>::iterator VECFGCARDIT;
typedef std::vector<FGCard>::const_iterator VECFGCARDCONSTIT;
#endif