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

    FGCard OutBoard(UINT nIndex);

    void AddBoard(const FGCard& fgBoard);

    UINT GetBoardNum() const;

private:
    vecFGCard m_vecFGCard;
    std::wstring m_strPlayerName;
};

#endif