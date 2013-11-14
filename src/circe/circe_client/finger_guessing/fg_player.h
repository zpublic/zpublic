#ifndef __GAME_FG_PLAYER_OBJECT_BASE
#define __GAME_FG_PLAYER_OBJECT_BASE

#include "fg_card_def.h"
#include "fg_player_base.h"

class CFGPlayer : public FGPlayerBase
{
public:
    CFGPlayer();

    ~CFGPlayer();

    void SetPlayerGame(const std::wstring& strName);

    const std::wstring& GetPlayerGame() const;

    BOOL OutCard(UINT nIndex, FGCard& card);

    BOOL GetCardInfo(UINT nIndex, FGCard& card);

    void AddCard(const FGCard& fgBoard);

    UINT GetCardNum() const;

private:
    vecFGCard m_vecCard;
    std::wstring m_strPlayerName;
};

#endif