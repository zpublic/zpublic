#ifndef __GAME_FG_PLAYER_OBJECT_BASE
#define __GAME_FG_PLAYER_OBJECT_BASE

#include "../player.h"
#include "game_object\board\fg_board.h"

class CFGPlayerGame : public CPlayerGameBase
{
public:
    CFGPlayerGame();

    ~CFGPlayerGame();

    void SetPlayerGame(const std::wstring& strName);

    const std::wstring& GetPlayerGame() const;

    FGBoard OutBoard(UINT nIndex);

    void AddBoard(const FGBoard& fgBoard);

    UINT GetBoardNum() const;

private:
    vecFGBoard m_vecFGBorad;
    std::wstring m_strPlayerName;
};

#endif