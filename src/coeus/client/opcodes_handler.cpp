#include "stdafx.h"
#include "opcodes_handler.h"
#include "game_logic.h"

#define MAKE_MESSAGE_HANDLER(m, f) \
    OpcodeHandler(m, std::bind(&GameLogic::f, std::placeholders::_1));

#define DEF_HANDLER(x, y) \
    _opcodeTable[Opcodes::x] = MAKE_MESSAGE_HANDLER(#x, y);


OpcodeTable::OpcodeTable()
{
    DEF_HANDLER(SCLoginRsp,              user_login_handler)
    DEF_HANDLER(SCRegisterRsp,           user_register_handler)
	DEF_HANDLER(SCCheckNicknameExistRsp, check_nickname_handler)
	DEF_HANDLER(SCGetRandomNameRsp,      rand_nickname_handler)
	DEF_HANDLER(SCCreateRoleRsp,         create_role_handler)
}
