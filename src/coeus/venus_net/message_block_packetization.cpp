#include "message_block_packetization.h"
#include "logger.h"
#include "message_notification.h"

MessageBlockPacketization::MessageBlockPacketization(const std::function<void (BasicStreamPtr&)>& callback)
    : _pendingStream(new BasicStream())
{
    _messageCallback = callback;
}
MessageBlockPacketization::~MessageBlockPacketization()
{

}

bool MessageBlockPacketization::appendBlock(const byte* buffer, size_t bytes_transferred)
{
    for (int32 readIdx = 0; readIdx < bytes_transferred; )
    {
        int32 leftLen = bytes_transferred - readIdx;

        //未达到长度的4字节则继续等待
        if (leftLen < NetworkParam::kMagicFlagLength && _pendingStream->b.size() == 0)
        {
            //把当前接收到的数据加入缓存
            addPending((const byte*)(buffer + readIdx), leftLen);
            return true;
        }

        //一个完整消息长度
        int32 msgLen = 0;

        //需要读的消息长度
        int32 needReadLen = 0;

        //存放该消息的字节流
        _packetStreamPtr = new BasicStream();

        //如果有缓存包
        if (_pendingStream->b.size() > 0)
        {
            //之前缓存不足4字节
            if (_pendingStream->b.size() < NetworkParam::kMagicFlagLength)
            {
                //现在还不足4字节，继续等
                if (_pendingStream->b.size() + leftLen < NetworkParam::kMagicFlagLength)
                {
                    //添加Pending,并返回
                    addPending((const byte*)(buffer + readIdx), leftLen);
                    return true;;
                }

                //如果已足4字节，先把4字节剩下的部分追加到PendingStream中
                int32 srcPendingLen = _pendingStream->b.size();
                _pendingStream->append((const byte*)buffer + readIdx, NetworkParam::kMagicFlagLength - _pendingStream->b.size());

                //读位置前移
                readIdx += (NetworkParam::kMagicFlagLength - srcPendingLen);

                //leftLen修正
                leftLen = bytes_transferred - readIdx;
            }

            //先从缓存包中读出包包表示的长度
            _pendingStream->i = _pendingStream->b.begin();
            _pendingStream->read(msgLen);

            //检查消息头合法性
            if (checkMessageLen(msgLen) == false) return false;

            //不是完整的包，设置中间的超时时间并继续等待
            if (leftLen < (int32)(msgLen - _pendingStream->b.size()))
            {
                //添加Pending，并返回
                addPending((const byte*)(buffer + readIdx), leftLen);

                // TODO: 设置下次接收超时时间
                // ...

                return true;
            }

            //剩下要读的数据长度
            needReadLen = msgLen - _pendingStream->b.size();

            //将PendingStream取出来
            _packetStreamPtr = _pendingStream;

            //重新开辟一个PendingStream存放残包
            _pendingStream = new BasicStream();
        }
        //没有缓存包
        else
        {
            //先读取消息长度
            msgLen = BasicStream::read((const byte*)buffer + readIdx);

            //检查消息长度的合法性
            if (checkMessageLen(msgLen) == false) return false;

            //如果剩余的字节流长度不足，则追加到Pending中并返回
            if (leftLen < msgLen)
            {
                addPending((const byte*)(buffer + readIdx), leftLen);
                return true;
            }
            needReadLen = msgLen;
        }

        //开始读消息体
        if (needReadLen > 0)
        {
            _packetStreamPtr->append((const byte*)(buffer + readIdx), needReadLen);
        }

        _packetStreamPtr->i = _packetStreamPtr->b.begin() + NetworkParam::kMagicFlagLength;
        byte comp = 0;
        //_packetStreamPtr->read(comp);

        //TODO:压缩预留
        if (comp > 0)
        {
        }

        //TODO::包加密预留
        byte encrypt = 0;
        //_packetStreamPtr->read(encrypt);
        if (encrypt > 0)
        {
        }

        // 反馈处理结果
        // 推送出去的包已经没有了长度，第一个4字节就是操作码
        _messageCallback(_packetStreamPtr);

        readIdx += needReadLen;
    }

    return true;
}

void MessageBlockPacketization::addPending(const byte* buff, size_t len)
{
    _pendingStream->append(buff, len);
}

bool MessageBlockPacketization::checkMessageLen(size_t len)
{
    if (len > NetworkParam::kMaxMessageLength)
    {
        error_log("message length too big");
        return false;
    }

    if (len < NetworkParam::kHeaderLength)
    {
        error_log("header length too small");
        return false;
    }

    return true;
}
