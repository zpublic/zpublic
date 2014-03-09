#include "game_session.h"
#include "venus_net/venus_net.h"
#include "game_common/game_define.h"
#include "game_database.h"
#include "player_db.h"
#include "player.h"
#include "player_manager.h"

void GameSession::getRandomNicknameHandler(const NetworkPacket::Ptr& packet)
{
    Protocol::CSGetRandomNameReq getRandomNicknameRequest;
    DECODE_MESSAGE(getRandomNicknameRequest, packet);

    uint8 gender = getRandomNicknameRequest.gender;
    Protocol::SCGetRandomNameRsp nicknameResponse;
    nicknameResponse.random_name = (gender == Gender::Female ? "·ßÅ­µÄÒ¦æ©" : "·ßÅ­µÄÅÝÃæ");

    this->send_message(Opcodes::SCGetRandomNameRsp, nicknameResponse);
}


void GameSession::checkNicknameExist(const NetworkPacket::Ptr& packet)
{
    Protocol::CSCheckNicknameExistReq checkNicknameExistRequest;
    DECODE_MESSAGE(checkNicknameExistRequest, packet);

    if (!checkNicknameExistRequest.nickname.empty())
    {
        Protocol::SCCheckNicknameExistRsp response;
        response.result = GameDatabase::getInstance().isNicknameExist(checkNicknameExistRequest.nickname);
        this->send_message(Opcodes::SCCheckNicknameExistRsp, response);
    }
}


void GameSession::characterCreateHandler(const NetworkPacket::Ptr& packet)
{
    Protocol::CSCreateCharacterReq createCharacterRequest;
    DECODE_MESSAGE(createCharacterRequest, packet);

    if (!createCharacterRequest.nickname.empty())
    {
        Protocol::SCCreateCharacterRsp response;
        response.result = (createCharacterRequest.belief > Belief::Holy && createCharacterRequest.belief < Belief::BeliefMax);
        response.result = (createCharacterRequest.gender == Gender::Female || createCharacterRequest.gender == Gender::Male);
        response.result = (createCharacterRequest.character_type > CharacterCareer::MirrorHunter && createCharacterRequest.character_type < CharacterCareer::CharacterCareerMax);
       
        if (response.result && GameDatabase::getInstance().createCharacter(
            _userGuid, 
            createCharacterRequest.character_type,
            createCharacterRequest.nickname,
            createCharacterRequest.gender,
            createCharacterRequest.belief))
        {
            Player* player = PlayerManager::getInstance().createPlayer(_userGuid, this);
            if (player != nullptr)
            {
                PlayerDB* playerDB = player->DB();
                playerDB->player_id = _userGuid;
                playerDB->belief = createCharacterRequest.belief;
                playerDB->character_type = createCharacterRequest.character_type;
                playerDB->gender = createCharacterRequest.gender;
            }

            this->send_message(Opcodes::SCCreateCharacterRsp, response);
        }
    }
}