#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "common.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/SocketConnector.h"
#include "Poco/NObserver.h"
#include "Poco/Thread.h"
#include "logger.h"
#include "message_block_packetization.h"
#include "network_message.h"

class MessageHandler
{
    static const int MAX_RECV_LEN = 1024 * 4;
public:
    MessageHandler(Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& reactor)
        : _buffer(new byte[MAX_RECV_LEN]),
        _reactor(reactor), 
        _socket(socket),
        _blockPacketization(std::bind(&MessageHandler::onMessage, this, std::placeholders::_1))
    {
    }

public:
    void onWritable(const Poco::AutoPtr<Poco::Net::WritableNotification>& notification)
    {
        //std::cout << "onWritable()" << std::endl;
    }

    void onReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& notification)
    {
        std::cout << "onReadable()" << std::endl;

        int bytes_transferred = _socket.receiveBytes(_buffer, MAX_RECV_LEN, 0);
        //debug_log("received %d bytes.", bytes_transferred);
        printf("received %d bytes.", bytes_transferred);
        if (bytes_transferred == 0)
        {
            _socket.close();
        }

        if (_blockPacketization.appendBlock(_buffer, bytes_transferred) == false)
        {
            _socket.close();
        }
    }

    void onShutdown(const Poco::AutoPtr<Poco::Net::ShutdownNotification>& notification)
    {
        std::cout << "onShutdown()" << std::endl;
        delete this;
    }

private:
    void onMessage(const BasicStreamPtr& packet)
    {
        //构造网络消息包给应用层
        // ...
        //Poco::Notification::Ptr notification(new MessageNotification(packet));
        //_messageQueue.enqueueNotification(notification);
    }

private:
    MessageBlockPacketization _blockPacketization;
    Poco::Net::SocketReactor& _reactor;
    Poco::Net::StreamSocket& _socket;
    byte* _buffer;

};

class TcpClient
{

public:
    TcpClient()
        : _messageHandler(_socket, _reactor), 
        _connector(nullptr)
    {
        _reactor.addEventHandler(_socket, Poco::NObserver<MessageHandler, Poco::Net::WritableNotification>(_messageHandler, &MessageHandler::onWritable));
        _reactor.addEventHandler(_socket, Poco::NObserver<MessageHandler, Poco::Net::ReadableNotification>(_messageHandler, &MessageHandler::onReadable));
        _reactor.addEventHandler(_socket, Poco::NObserver<MessageHandler, Poco::Net::ShutdownNotification>(_messageHandler, &MessageHandler::onShutdown));
    }

    virtual ~TcpClient()
    {
        _reactor.removeEventHandler(_socket, Poco::NObserver<MessageHandler, Poco::Net::WritableNotification>(_messageHandler, &MessageHandler::onWritable));
        _reactor.removeEventHandler(_socket, Poco::NObserver<MessageHandler, Poco::Net::ReadableNotification>(_messageHandler, &MessageHandler::onReadable));
        _reactor.removeEventHandler(_socket, Poco::NObserver<MessageHandler, Poco::Net::ShutdownNotification>(_messageHandler, &MessageHandler::onShutdown));
        _reactor.stop();
    }

    bool connect(Poco::Net::SocketAddress& address, const Poco::Timespan& timeout = Poco::Timespan(50000))
    {
        try
        {
            if (_connector != nullptr)
            {
                SAFE_DELETE(_connector);
            }

            Poco::Thread thread;
            thread.start(_reactor);
            _socket.close();
            _socket.connectNB(address);
        }
        catch (Poco::TimeoutException& e)
        {
            std::cout << e.displayText() << std::endl;
            return false;
        }
        catch (Poco::Exception& e)
        {
            std::cout << e.displayText() << std::endl;
            return false;
        }

        return true;
    }

    void sendMessage(const BasicStreamPtr& stream)
    {
        bool writeable = _socket.poll(0, Poco::Net::Socket::SelectMode::SELECT_WRITE);
        bool error = _socket.poll(0, Poco::Net::Socket::SelectMode::SELECT_ERROR);

        if (writeable && !error)
        {
            _socket.sendBytes((const void*)stream->b.begin(), stream->b.size());
        }
    }

    void sendMessage(int16 opcode, const byte* buff, size_t size)
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

    void sendMessage(uint16 opcode, NetworkMessage& message)
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

private:
    MessageHandler _messageHandler;
    Poco::Net::SocketReactor _reactor;
    Poco::Net::StreamSocket _socket;
    Poco::Net::SocketConnector<MessageHandler>* _connector;
};

#endif // !__TCP_CLIENT_H__