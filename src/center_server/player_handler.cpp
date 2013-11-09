#include "game_session.h"
#include "player.h"
#include "player_db.h"

void GameSession::get_player_profile_handler(const NetworkMessage& message)
{
    Protocol::C2SGetPlayerProfileReq request;
    PARSE_NETWORK_MESSAGE(message, request);

    Protocol::S2CGetPlayerProfileRsp response;
    response.set_nickname(_player->nickname());
    response.set_gender(_player->gender());
    response.set_guid(_player->guid());
    response.set_guild_id(_player->guildId());
    response.set_guild_name(_player->guildName());
    response.set_last_login(_player->lastLogin());

    send_message<Protocol::S2CGetPlayerProfileRsp>(Opcodes::S2CGetPlayerProfileRsp, response);
}