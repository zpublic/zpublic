#include "opcode_registry.h"

// #define MAKE_MESSAGE_HANDLER(m, f) \
//     OpcodeHandler(m, std::bind(&GameSession::f, std::placeholders::_1, std::placeholders::_2));
//     
// #define DEFINE_HANDLER(m, f) \
// 	_opcodeTable[Opcodes::m] = MAKE_MESSAGE_HANDLER(#m, f);

// OpcodeTable::OpcodeTable()
// {
// 	DEFINE_HANDLER(C2SHeartbeat, heartbeat_handler);
//     DEFINE_HANDLER(C2SLoginReq, user_login_handler);
//     DEFINE_HANDLER(C2SRegisterReq, user_register_handler);
//     DEFINE_HANDLER(C2SGetPlayerProfileReq, get_player_profile_handler);
//     DEFINE_HANDLER(C2SChatMessageReq, chat_message_handler);
//     DEFINE_HANDLER(C2SRoomCreateReq, room_create_handler);
//     DEFINE_HANDLER(C2SSRoomInfoChangeReq, room_info_change_handler);
//     DEFINE_HANDLER(C2SGetRoomListReq, get_room_list_handler);
//     DEFINE_HANDLER(C2SEnterRoomReq, enter_room_handler);
// }