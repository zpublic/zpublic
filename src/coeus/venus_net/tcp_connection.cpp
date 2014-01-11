#include "tcp_connection.h"
#include "Poco/Util/Application.h"
#include "Poco/Timespan.h"
#include "service_application.h"
#include "logger.h"
#include "message_notification.h"
#include "stream_writer.h"
#include "stream_reader.h"
#include "message_queue.h"
#include "server_connection.h"

TcpConnection::TcpConnection(const Poco::Net::StreamSocket& socket, MessageQueue& messageQueue, uint32 sequence)
    : Poco::Net::TCPServerConnection(socket),
    _socket(const_cast<Poco::Net::StreamSocket&>(socket)),
    _buffer(new byte[MAX_RECV_LEN]),
    _blockPacketization(std::bind(&TcpConnection::finishedPacketCallback, this, std::placeholders::_1)),
    _messageQueue(messageQueue),
    _sequence(sequence),
    _serverConnection(nullptr)
{
    _socket.setBlocking(false);
    _socket.setReuseAddress(true);
    _socket.setReusePort(true);
}

TcpConnection::~TcpConnection()
{
    SAFE_DELETE_ARR(_buffer);
    SAFE_DELETE(_serverConnection);
    debug_log("connection destroyed.");
}

void TcpConnection::run()
{
    try
    {
        _serverConnection = new ServerConnection(this);
        _messageQueue.dispatcher()->onNewConnection(_serverConnection);
        
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
        error_log("Poco::Exception : %s", e.what());
        onShutdown(ShutdownReason::SR_EXCEPTION);
    }
    catch (...)
    {
        error_log("unknown exception.");
        onShutdown(ShutdownReason::SR_EXCEPTION);
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
    //debug_log("received %d bytes.", bytes_transferred);
    if (bytes_transferred == 0)
    {
        onShutdown(ShutdownReason::SR_PEER_GRACEFUL_SHUTDOWN);
        return false;
    }

    return _blockPacketization.appendBlock(_buffer, bytes_transferred);
}

void TcpConnection::onShutdown(const ShutdownReason& reason)
{
    switch (reason)
    {
    case ShutdownReason::SR_SERVICE_CLOSE_INITIATIVE:
        {
            debug_log("service close connection initiative.");
            break;
        }
    case ShutdownReason::SR_PEER_GRACEFUL_SHUTDOWN:
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

    _messageQueue.dispatcher()->onShutdown(_serverConnection, reason);
    _socket.shutdown();
    _socket.close();
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
    Poco::Notification::Ptr notification(new MessageNotification(_serverConnection, packetPtr));
    _messageQueue.enqueueNotification(notification);
}

void TcpConnection::close(const ShutdownReason& reason)
{
    onShutdown(reason);
}