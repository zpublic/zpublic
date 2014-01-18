/* -------------------------------------------------------------------------
//  File        :   game_hander.h
//  Author      :   Sum
//  Datet       :   2014/1/10 20:05
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __GAME_HANDER_H_
#define __GAME_HANDER_H_

#include "venus_net/tcp_client.h"
#include "opcodes_handler.h"

class GameMessageHandler : public MessageHandler
{
public:
    virtual void onConnected()
    {
    }

    virtual void onMessage(uint16 opcode, const NetworkPacket::Ptr& message)
    {
        OpcodeHandler* pHandler = OpcodeTable::instance()[opcode];

        if (pHandler != NULL)
        {
            pHandler->message_handler(message);
        }
    }

    virtual void onShutdown()
    {
    }
};

#endif // __GAME_HANDER_H_