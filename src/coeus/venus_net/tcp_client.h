#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "common.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/SocketConnector.h"
#include "Poco/NObserver.h"
#include "Poco/Observer.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "logger.h"
#include "message_block_packetization.h"
#include "network_common.h"

class TcpClient;
class MessageHandler
{
public:
    virtual void onConnected() = 0;
    virtual void onMessage(uint16 opcode, const NetworkPacket::Ptr& message) = 0;
    virtual void onShutdown() = 0;
};

class TcpClient
{
    static const int MAX_RECV_LEN = 1024 * 4;

public:
    TcpClient(MessageHandler& handler);
    virtual ~TcpClient();

    bool connect(Poco::Net::SocketAddress& address, const Poco::Timespan& timeout = Poco::Timespan(50000));
    inline bool connected() const { return _isConnected; }
    void close();
    void sendMessage(int16 opcode, const byte* buff, size_t size);
    void sendMessage(uint16 opcode, NetworkMessage& message);

private:
    void sendMessage(const BasicStreamPtr& stream);
    void resetNetwork();

private:
    void onWritable(Poco::Net::WritableNotification* notification);
    void onReadable(Poco::Net::ReadableNotification* notification);
    void onShutdown(Poco::Net::ShutdownNotification* notification);
    void onIdle(Poco::Net::IdleNotification* notification);
    void onTimeout(Poco::Net::TimeoutNotification* notification);
    void finishedPacketCallback(BasicStreamPtr& packet);

private:
    bool _isConnected;
    Poco::Net::SocketReactor* _reactor;
    Poco::Net::StreamSocket* _socket;
    Poco::Thread* _reactorThread;
    MessageBlockPacketization _blockPacketization;
    byte* _buffer;
    MessageHandler& _handler;
};

#endif // !__TCP_CLIENT_H__