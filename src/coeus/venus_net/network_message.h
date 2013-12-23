#ifndef __NETWORK_MESSAGE_H__
#define __NETWORK_MESSAGE_H__

#include "common.h"

struct NetworkMessage
{
public:
    const static uint8 kMagicFlagLength = 4;            //消息包最前面表示包总长度的字节数
    const static uint8 kHeaderLength = 8;               //长度和操作码
    const static int32 kMaxMessageLength = 10 * 65535;  //消息最大长度

    NetworkMessage(){}
    virtual ~NetworkMessage(){}

    virtual int32 byteSize() = 0;
    virtual void encode(byte* buffer, size_t size) = 0;
    virtual void decode(const byte* buffer, size_t size) = 0;
};

#endif // !__NETWORK_MESSAGE_H__
