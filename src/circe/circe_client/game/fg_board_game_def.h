#ifndef __GAME_FG_BOARD_GAME_DEF
#define __GAME_FG_BOARD_GAME_DEF

#include "game_object\board\fg_board.h"
#include "game_object\player_game\fg\fg_player_game.h"
#include "game_object\rule_game\fg_rule_board.h"
#include <vector>
#include <string>

typedef std::vector<CFGPlayerGame*> vecFGPlayerGame;
typedef std::vector<CFGPlayerGame*>::iterator vecFGPlayerGameIt;
typedef std::vector<CFGPlayerGame*>::const_iterator vecFGPlayerGameConstIt;

#endif