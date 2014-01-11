#ifndef __NETWORK_MESSAGE_H__
#define __NETWORK_MESSAGE_H__

#include "common.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"

struct NetworkParam
{
    const static uint8 kMagicFlagLength = 4;            //消息包最前面表示包总长度的字节数
    const static uint8 kHeaderLength = 8;               //长度和操作码
    const static int32 kMaxMessageLength = 10 * 65535;  //消息最大长度
};

// 所有实例化了的消息类均继承此类
// 消息类需要重写byteSize/encode/decode三个方法
class NetworkMessage
{
public:
    NetworkMessage(){}
    virtual ~NetworkMessage(){}

    virtual int32 byteSize() = 0;
    virtual void encode(byte* buffer, size_t size) = 0;
    virtual void decode(const byte* buffer, size_t size) = 0;
};

// NetworkPacket类用于包装网络收到的数据流
// 推送到上层时需要从NetworkPacket中取出流进行反序列化得到具体的消息结构
struct NetworkPacket : Poco::RefCountedObject
{
    typedef Poco::AutoPtr<NetworkPacket> Ptr;
    typedef std::vector<byte> PDU;

    uint16 opcode;
    PDU messageBody;
};

#define DECODE_MESSAGE(message, data) \
    message.decode((const byte*)&data->messageBody[0], data->messageBody.size());


#endif // !__NETWORK_MESSAGE_H__
