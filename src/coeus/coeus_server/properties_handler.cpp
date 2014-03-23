#include "game_session.h"

void GameSession::getPropertiesHandler(const NetworkPacket::Ptr& packet)
{
    Protocol::CSPropertiesReq propertiesRequest;
    DECODE_MESSAGE(propertiesRequest, packet);

    Protocol::SCPropertiesRsp response;
    response.coins = 500;
    response.exp = 0;
    response.level = 1;

    send_message(Opcodes::SCPropertiesRsp, response);
}