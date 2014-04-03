#ifndef __NETWORK_COMMON_H__
#define __NETWORK_COMMON_H__

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
    PDU message;
};

#define DECODE_MESSAGE(network_message, data) \
    if (!data->message.empty()) network_message.decode((const byte*)&data->message[0], data->message.size());

// 网络连接关闭原因
enum ShutdownReason
{
    SR_SERVICE_CLOSE_INITIATIVE, // 服务主动关闭连接（无理由）
    SR_SERVICE_PACKET_FAILURE,   // 解包失败
    SR_KICK_OUT,                 // 服务主动踢掉某个连接
    SR_SERVICE_STOP,             // 服务需要停止
    SR_PEER_GRACEFUL_SHUTDOWN,   // 客户端安全关闭连接
    SR_EXCEPTION                 // 发生未知异常（可能是RESET）
};

#endif // !__NETWORK_COMMON_H__