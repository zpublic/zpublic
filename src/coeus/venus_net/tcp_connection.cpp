#include "tcp_connection.h"
#include "Poco/Util/Application.h"
#include "Poco/Timespan.h"
#include "service_application.h"
#include "logger.h"
#include "message_notification.h"
#include "stream_writer.h"
#include "stream_reader.h"
#include "message_queue.h"

struct CSTestPacketRsp : public NetworkMessage
{
    uint32 uint_value;
    std::string string_value;

    int byteSize()
    {
        return sizeof(uint_value) + (string_value.length() + 2);
    }

    void encode(byte* buffer, size_t size)
    {
        StreamWriter w((char*)buffer, size);
        w << uint_value;
        w << string_value;
    }

    void decode(const byte* buffer, size_t size)
    {
        StreamReader r((const char*)buffer, size);
        r >> uint_value;
        r >> string_value;
    }
};

TcpConnection::TcpConnection(const Poco::Net::StreamSocket& socket, MessageQueue& messageQueue)
    : Poco::Net::TCPServerConnection(socket),
    _socket(const_cast<Poco::Net::StreamSocket&>(socket)),
    _buffer(new byte[MAX_RECV_LEN]),
    _blockPacketization(std::bind(&TcpConnection::finishedPacketCallback, this, std::placeholders::_1)),
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
        debug_log("connection established.");
        CSTestPacketRsp requestMessage;
        requestMessage.uint_value = 20;
        requestMessage.string_value = "i am server";
        sendMessage(22222, requestMessage);
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

    streamPtr->resize(NetworkParam::kHeaderLength + message.byteSize());
    message.encode((byte*)streamPtr->b.begin() + NetworkParam::kHeaderLength, message.byteSize());
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

    return _blockPacketization.appendBlock(_buffer, bytes_transferred);
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
            debug_log("connection exception, maybe reset from peer.");
            break;
        }
    default:
        break;
    }
}

void TcpConnection::finishedPacketCallback(BasicStreamPtr& packet)
{
    //构造网络消息包给应用层
    uint16 opcode = 0;
    packet->read(opcode);

    NetworkPacket::Ptr packetPtr(new NetworkPacket);
    packetPtr->opcode = opcode;
    packetPtr->messageBody = NetworkPacket::PDU(packet->b.begin() + NetworkParam::kHeaderLength, packet->b.end());

    //构造网络消息包给应用层
    // ...
    Poco::Notification::Ptr notification(new MessageNotification(packetPtr));
    _messageQueue.enqueueNotification(notification);
}