#include <google/protobuf/message.h>
#include "byte_buffer.h"

#pragma pack(push, 1)

struct SERVER_DECL ServerPacket
{
    const static int MIN_HEADER_LENGTH = sizeof(int);

    size_t len;
    uint32_t opcode;
    google::protobuf::Message* message;

    ServerPacket() : message(NULL) {}

    __forceinline size_t byteSize() const 
    {
        if (message == NULL)
            return 0;

        return sizeof(len) + sizeof(opcode) + message->ByteSize();
    }
};

#pragma pack(pop)