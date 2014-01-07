#ifndef __GAME_OPCODE_REGISTRY_H__
#define __GAME_OPCODE_REGISTRY_H__

#include "venus_net/opcode_registry.h"
#include "game_session.h"

class GameOpcodeRegistry
    : public Venus::OpcodeRegistry<GameSession>
{
    void initialize();
};

#endif