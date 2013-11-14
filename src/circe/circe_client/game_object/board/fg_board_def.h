#ifndef __GAME_FG_BOARD_OBJECT_DEF
#define __GAME_FG_BOARD_OBJECT_DEF

typedef enum _emBoardType
{
    emBoardType_Null,
    emBoardType_Stone,
    emBoardType_Scissor,
    emBoardType_Cloth,
}emBoardType;

#include <vector>
#include <string>

struct FGBoard;

typedef std::vector<FGBoard> vecFGBoard;
typedef std::vector<FGBoard>::iterator vecFGBoardIt;
typedef std::vector<FGBoard>::const_iterator vecFGBoardConstIt;
#endif