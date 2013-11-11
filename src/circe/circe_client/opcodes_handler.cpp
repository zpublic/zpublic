#include "stdafx.h"
#include "opcodes_handler.h"
#include "game_handler.h"

#define MAKE_MESSAGE_HANDLER(m, f) \
    OpcodeHandler(m, std::bind(&GameHandler::f, std::placeholders::_1));

#define DEF_HANDLER(x, y) \
    _opcodeTable[Opcodes::x] = MAKE_MESSAGE_HANDLER(#x, y);


OpcodeTable::OpcodeTable()
{
    DEF_HANDLER(S2CLoginRsp,    user_login_handler)
    DEF_HANDLER(S2CRegisterRsp, user_register_handler)
}
