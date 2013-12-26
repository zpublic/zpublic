#ifndef __MESSAGE_BLOCK_PACKETIZATION_H__
#define __MESSAGE_BLOCK_PACKETIZATION_H__

#include "common.h"
#include "message_queue.h"
#include "basic_stream.h"

class MessageBlockPacketization
{
public:
    MessageBlockPacketization(MessageQueue& messageQueue);
    virtual ~MessageBlockPacketization();

public:
    bool appendBlock(const byte* buffer, size_t size);

private:
    void addPending(const byte* buff, size_t len);
    bool checkMessageLen(size_t len);

private:
    BasicStreamPtr _pendingStream;      //存放接收到的数据
    BasicStreamPtr _packetStreamPtr;    //存放一个完整的数据包
    MessageQueue& _messageQueue;
};

#endif // !__MESSAGE_BLOCK_PACKETIZATION_H__
