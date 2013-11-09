#include "opcodes.h"
#include "opcodes_handler.h"
#include "game_session.h"

#define MAKE_MESSAGE_HANDLER(m, f) \
    OpcodeHandler(m, std::bind(&GameSession::f, std::placeholders::_1, std::placeholders::_2));
    

OpcodeTable::OpcodeTable()
{
    _opcodeTable[Opcodes::C2SLoginReq] = MAKE_MESSAGE_HANDLER("C2SLoginReq", user_login_handler);
    _opcodeTable[Opcodes::C2SRegisterReq] = MAKE_MESSAGE_HANDLER("C2SRegisterReq", user_register_handler);
    _opcodeTable[Opcodes::C2SGetPlayerProfileReq] = MAKE_MESSAGE_HANDLER("C2SGetPlayerProfileReq", get_player_profile_handler);
    _opcodeTable[Opcodes::C2SChatMessageReq] = MAKE_MESSAGE_HANDLER("C2SChatMessageReq", chat_message_handler);
}