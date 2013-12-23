#include "tcp_connection.h"
#include "Poco/Util/Application.h"
#include "Poco/Timespan.h"
#include "service_application.h"
#include "logger.h"
#include "message_notification.h"

TcpConnection::TcpConnection(const Poco::Net::StreamSocket& socket, MessageQueue& messageQueue)
    : Poco::Net::TCPServerConnection(socket),
    _socket(const_cast<Poco::Net::StreamSocket&>(socket)),
    _buffer(new byte[MAX_RECV_LEN]),
    _pendingStream(new BasicStream()),
    _messageQueue(messageQueue)
{
    _socket.setBlocking(false);
    _socket.setReuseAddress(true);
    _socket.setReusePort(true);
}

TcpConnection::~TcpConnection()
{
    SAFE_DELETE_ARR(_buffer);
    debug_log("connection destroyed.");
}

void TcpConnection::run()
{
    try
    {
        //Poco::Net::SocketAddress address = _socket.peerAddress();
        //std::string peer(_socket.peerAddress().toString().data());

        //char hostname[48] = {0};
        //memcpy(hostname, _socket.peerAddress().toString().c_str(), strlen(_socket.peerAddress().toString().c_str()));
        debug_log("connection established.");
        sendMessage(10001, (const byte*)"hello", 5);
        for (;;)
        {
			bool readable = _socket.poll(Poco::Timespan(30, 0), Poco::Net::Socket::SelectMode::SELECT_READ);
			if (readable == true && onReadable() == false)
            {
                return;
            }
        }
    }
    catch (Poco::Exception& e)
    {
        onShutdown(ShutdownReason::SR_EXCEPTION);
    }
    catch (...)
    {
        error_log("unknown exception.");
    }
}


void TcpConnection::sendMessage(const BasicStreamPtr& stream)
{
    bool writeable = _socket.poll(0, Poco::Net::Socket::SelectMode::SELECT_WRITE);
    bool error = _socket.poll(0, Poco::Net::Socket::SelectMode::SELECT_ERROR);

    if (writeable && !error)
    {
        _socket.sendBytes((const void*)stream->b.begin(), stream->b.size());
    }
}

void TcpConnection::sendMessage(int16 opcode, const byte* buff, size_t size)
{
    BasicStreamPtr streamPtr(new BasicStream());

    streamPtr->write((int32)0); //长度预留
    streamPtr->write(opcode);   //操作码
    // ...
    // TODO: 包压缩和加密标志预留

    streamPtr->append((const byte*)buff, size);
    streamPtr->rewriteSize(streamPtr->b.size(), streamPtr->b.begin());

    sendMessage(streamPtr);
}

void TcpConnection::sendMessage(uint16 opcode, NetworkMessage& message)
{
    BasicStreamPtr streamPtr(new BasicStream());
    streamPtr->write((int32)0);
    streamPtr->write(opcode);
    // ...
    // TODO: 包压缩和加密标志预留

    streamPtr->resize(NetworkMessage::kHeaderLength + message.byteSize());
    message.encode((byte*)streamPtr->b.begin() + NetworkMessage::kHeaderLength, message.byteSize());
    streamPtr->rewriteSize(streamPtr->b.size(), streamPtr->b.begin());

    sendMessage(streamPtr);
}

// 返回false意味着连接已被对端关闭
bool TcpConnection::onReadable()
{
    int bytes_transferred = _socket.receiveBytes(_buffer, MAX_RECV_LEN, 0);
    debug_log("received %d bytes.", bytes_transferred);
    if (bytes_transferred == 0)
    {
        onShutdown(ShutdownReason::SR_GRACEFUL_SHUTDOWN);
        return false;
    }

    for (int32 readIdx = 0; readIdx < bytes_transferred; )
    {
        int32 leftLen = bytes_transferred - readIdx;

        //未达到长度的4字节则继续等待
        if (leftLen < NetworkMessage::kMagicFlagLength && _pendingStream->b.size() == 0)
        {
            //把当前接收到的数据加入缓存
            addPending((const byte*)(_buffer + readIdx), leftLen);
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
            if (_pendingStream->b.size() < NetworkMessage::kMagicFlagLength)
            {
                //现在还不足4字节，继续等
                if (_pendingStream->b.size() + leftLen < NetworkMessage::kMagicFlagLength)
                {
                    //添加Pending,并返回
                    addPending((const byte*)(_buffer + readIdx), leftLen);
                    return true;
                }

                //如果已足4字节，先把4字节剩下的部分追加到PendingStream中
                int32 srcPendingLen = _pendingStream->b.size();
                _pendingStream->append((const byte*)_buffer + readIdx, NetworkMessage::kMagicFlagLength - _pendingStream->b.size());

                //读位置前移
                readIdx += (NetworkMessage::kMagicFlagLength - srcPendingLen);

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
                addPending((const byte*)(_buffer + readIdx), leftLen);

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
            msgLen = BasicStream::read((const byte*)_buffer + readIdx);

            //检查消息长度的合法性
            if (checkMessageLen(msgLen) == false) return false;

            //如果剩余的字节流长度不足，则追加到Pending中并返回
            if (leftLen < msgLen)
            {
                addPending((const byte*)(_buffer + readIdx), leftLen);
                return true;
            }
            needReadLen = msgLen;
        }

        //开始读消息体
        if (needReadLen > 0)
        {
            _packetStreamPtr->append((const byte*)(_buffer + readIdx), needReadLen);
        }

        _packetStreamPtr->i = _packetStreamPtr->b.begin() + NetworkMessage::kMagicFlagLength;
        byte comp = 0;
        _packetStreamPtr->read(comp);

        //TODO:压缩预留
        if (comp > 0)
        {
        }

        //TODO::包加密预留
        byte encrypt = 0;
        _packetStreamPtr->read(encrypt);
        if (encrypt > 0)
        {
        }

        //构造网络消息包给应用层
        // ...
        Poco::Notification::Ptr notification(new MessageNotification(_packetStreamPtr));
        _messageQueue.enqueueNotification(notification);

        readIdx += needReadLen;
    }

    return true;
}

void TcpConnection::onShutdown(const ShutdownReason& reason)
{
    switch (reason)
    {
    case ShutdownReason::SR_GRACEFUL_SHUTDOWN:
        {
            debug_log("connection graceful shutdown from the peer.");
            break;
        }
    case ShutdownReason::SR_EXCEPTION:
        {
            debug_log("connection exception.");
            break;
        }
    default:
        break;
    }
}

void TcpConnection::addPending(const byte* buff, size_t len)
{
    _pendingStream->append(buff, len);
}

bool TcpConnection::checkMessageLen(size_t len)
{
    if (len > NetworkMessage::kMaxMessageLength)
    {
        error_log("message length too big");
        return false;
    }

    if (len < NetworkMessage::kHeaderLength)
    {
        error_log("header length too small");
        return false;
    }

    return true;
}