#ifndef __GAME_FG_PLAYER_OBJECT_BASE
#define __GAME_FG_PLAYER_OBJECT_BASE

#include "../player_game.h"
#include "game_object\board\fg_board.h"
#include <vector>
#include <string>

typedef std::vector<CFGBoard> vecFGBoard;
typedef std::vector<CFGBoard>::iterator vecFGBoardIt;
typedef std::vector<CFGBoard>::const_iterator vecFGBoardConstIt;

class CFGPlayerGame : public CPlayerGameBase
{
public:
    CFGPlayerGame();

    ~CFGPlayerGame();

    void SetPlayerGame(const std::wstring& strName);

    const std::wstring& GetPlayerGame() const;

    CFGBoard OutBoard(UINT nIndex);

    void AddBoard(const CFGBoard& fgBoard);

    UINT GetBoardNum() const;

private:
    vecFGBoard m_vecFGBorad;
    std::wstring m_strPlayerName;
};

#endif