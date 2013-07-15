#include <google/protobuf/message.h>

#pragma pack(push, 1)

struct Packet
{
    int len;
    short opcode;
    unsigned char* message;
};

#pragma pack(pop)