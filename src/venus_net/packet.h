#ifndef PACKET_H_
#define PACKET_H_

#include <google/protobuf/message.h>

#pragma pack(push, 1)

struct SERVER_DECL ServerPacket
{
    const static int HEADER_LENGTH = sizeof(size_t) + sizeof(size_t);
    const static int MIN_PACKET_LENGTH = sizeof(size_t) + sizeof(size_t) + sizeof(size_t);

    ServerPacket() : len(0), opcode(0), message(NULL)
    {
    }

    ~ServerPacket()
    {
        SAFE_DELETE_ARR(message);
        debug_log("packet destroy safely.");
    }

    size_t len;
    uint32_t opcode;
    byte* message;

};

#pragma pack(pop)

#endif