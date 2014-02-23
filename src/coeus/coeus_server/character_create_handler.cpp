#include "game_session.h"
#include "venus_net/venus_net.h"
#include "game_common/game_define.h"

void GameSession::getRandomNicknameHandler(const NetworkPacket::Ptr& packet)
{
    Protocol::CSGetRandomNameReq getRandomNicknameRequest;
    DECODE_MESSAGE(getRandomNicknameRequest, packet);

    uint8 gender = getRandomNicknameRequest.gender;
    Protocol::SCGetRandomNameRsp nicknameResponse;
    nicknameResponse.random_name = (gender == GENDER_FEMALE ? "·ßÅ­µÄÒ¦æ©" : "·ßÅ­µÄÅÝÃæ");

    this->send_message(Opcodes::SCGetRandomNameRsp, nicknameResponse);
}