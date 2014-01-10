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

class GameMessageHandler : public MessageHandler
{
public:
    virtual void onConnected()
    {
    }

    virtual void opcodenMessage(uint16 opcode, const NetworkPacket::Ptr& message)
    {
//         printf("onMessage() : [opcode = %d]\n", opcode);
//         CSTestPacketRsp requestMessage;
//         requestMessage.decode((const byte*)&message->messageBody[0], message->messageBody.size());
//         printf("        [value = %d]\n", requestMessage.uint_value);
//         printf("        [string = %s]\n", requestMessage.string_value.c_str());
    }

    virtual void onShutdown()
    {
    }
};

#endif // __GAME_HANDER_H_