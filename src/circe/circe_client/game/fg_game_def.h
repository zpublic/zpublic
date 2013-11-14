#ifndef __GAME_FG_BOARD_GAME_DEF
#define __GAME_FG_BOARD_GAME_DEF

#include "game_object\board\fg_board.h"
#include "game_object\player\fg\fg_player.h"
#include "game_object\rule\fg_rule_shuffle.h"
#include <vector>
#include <string>

typedef std::vector<CFGPlayer*> vecFGPlayerGame;
typedef std::vector<CFGPlayer*>::iterator vecFGPlayerGameIt;
typedef std::vector<CFGPlayer*>::const_iterator vecFGPlayerGameConstIt;

#endif