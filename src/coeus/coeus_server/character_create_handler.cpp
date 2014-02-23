#include "game_session.h"
#include "venus_net/venus_net.h"
#include "game_common/game_define.h"

void GameSession::getRandomNicknameHandler(const NetworkPacket::Ptr& packet)
{
    Protocol::CSGetRandomNameReq getRandomNicknameRequest;
    DECODE_MESSAGE(getRandomNicknameRequest, packet);

    uint8 gender = getRandomNicknameRequest.gender;

    Protocol::SCGetRandomNameRsp nicknameResponse;
    if (gender == GENDER_FEMALE)
    {
        nicknameResponse.random_name = "·ßÅ­µÄÒ¦æ©";
    }
    else
    {
        nicknameResponse.random_name = "·ßÅ­µÄÅÝÃæ";
    }

    this->send_message(Opcodes::SCGetRandomNameRsp, nicknameResponse);
}