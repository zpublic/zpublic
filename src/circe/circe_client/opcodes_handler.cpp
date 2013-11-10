#include "stdafx.h"
#include "opcodes.h"
#include "opcodes_handler.h"
#include "game_handler.h"

#define MAKE_MESSAGE_HANDLER(m, f) \
    OpcodeHandler(m, std::bind(&GameHandler::f, std::placeholders::_1));


OpcodeTable::OpcodeTable()
{
    _opcodeTable[Opcodes::S2CLoginRsp] = MAKE_MESSAGE_HANDLER("S2CLoginRsp", user_login_handler);
    _opcodeTable[Opcodes::C2SRegisterReq] = MAKE_MESSAGE_HANDLER("C2SRegisterReq", user_register_handler);
}
