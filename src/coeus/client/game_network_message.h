/* -------------------------------------------------------------------------
//  File        :   game_network_message.h
//  Author      :   Sum
//  Datet       :   2014/1/10 21:10
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __GAME_NETWORK_MESSAGE_H_
#define __GAME_NETWORK_MESSAGE_H_

#include "venus_net/network_common.h"

class GameNetworkMessage : public NetworkMessage
{
public:
    GameNetworkMessage() : m_nId(0) {}
    ~GameNetworkMessage() {}

    int byteSize()
    {
        return 0;
    }

    void encode(byte* buffer, size_t size)
    {
    }

    void decode(const byte* buffer, size_t size)
    {
    }

    int m_nId;
};

#endif // __GAME_NETWORK_MESSAGE_H_