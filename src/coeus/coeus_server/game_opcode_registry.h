#ifndef __GAME_OPCODE_REGISTRY_H__
#define __GAME_OPCODE_REGISTRY_H__

#include "venus_net/opcode_registry.h"
#include "game_session.h"

OPCODE_REGISTER_BEGIN(GameOpcodeRegistry, GameSession)
	REGISTER_HANDLER(Opcodes::CSRegisterReq, registerHandler);
	REGISTER_HANDLER(Opcodes::CSLoginReq, loginHandler);
    REGISTER_HANDLER(Opcodes::CSGetRandomNameReq, getRandomNicknameHandler);
    REGISTER_HANDLER(Opcodes::CSCheckNicknameExistReq, checkNicknameExist);
    REGISTER_HANDLER(Opcodes::CSCreateCharacterReq, characterCreateHandler);
    REGISTER_HANDLER(Opcodes::CSPropertiesReq, getPropertiesHandler);
OPCODE_REGISTER_END()

#endif