#ifndef PACKET_H_
#define PACKET_H_

#include <google/protobuf/message.h>

#pragma pack(push, 1)

struct SERVER_DECL ServerPacket
{
    const static int MIN_HEADER_LENGTH = sizeof(size_t);

    size_t len;
    uint32_t opcode;
    byte message[sizeof(google::protobuf::Message)];

    const google::protobuf::Message* protoMessage() const
    {
        return (google::protobuf::Message*)message;
    }

    ServerPacket() : len(0), opcode(0) 
    {
        memset(message, 0, sizeof(google::protobuf::Message));
    }
};

#pragma pack(pop)

#endif