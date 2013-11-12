#include "opcodes.h"
#include "opcodes_handler.h"
#include "game_session.h"

#define MAKE_MESSAGE_HANDLER(m, f) \
    OpcodeHandler(m, std::bind(&GameSession::f, std::placeholders::_1, std::placeholders::_2));
    
#define DEFINE_HANDLER(m, f) \
	_opcodeTable[m] = MAKE_MESSAGE_HANDLER(#m, f);

OpcodeTable::OpcodeTable()
{
    DEFINE_HANDLER(Opcodes::C2SLoginReq, user_login_handler);
    DEFINE_HANDLER(Opcodes::C2SRegisterReq, user_register_handler);
    DEFINE_HANDLER(Opcodes::C2SGetPlayerProfileReq, get_player_profile_handler);
    DEFINE_HANDLER(Opcodes::C2SChatMessageReq, chat_message_handler);
    DEFINE_HANDLER(Opcodes::C2SRoomCreateReq, room_create_handler);
}