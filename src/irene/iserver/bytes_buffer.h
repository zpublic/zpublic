#include <google/protobuf/message.h>
#include "common_def.h"

#pragma pack(push, 1)

struct PDU
{
    int len;
    short opcode;
    byte body[];
};

#pragma pack(pop)
