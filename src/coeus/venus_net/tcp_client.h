#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "common.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/SocketConnector.h"
#include "Poco/NObserver.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "logger.h"
#include "message_block_packetization.h"
#include "network_message.h"

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
    void close();
    void sendMessage(const BasicStreamPtr& stream);
    void sendMessage(int16 opcode, const byte* buff, size_t size);
    void sendMessage(uint16 opcode, NetworkMessage& message);

private:
    void resetNetwork();

private:
    void onWritable(const Poco::AutoPtr<Poco::Net::WritableNotification>& notification);
    void onReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& notification);
    void onShutdown(const Poco::AutoPtr<Poco::Net::ShutdownNotification>& notification);
    void onTimeout(const Poco::AutoPtr<Poco::Net::TimeoutNotification>& notification);
    void finishedPacketCallback(BasicStreamPtr& packet);

private:
    Poco::Net::SocketReactor* _reactor;
    Poco::Net::StreamSocket* _socket;
    Poco::Thread* _reactorThread;
    MessageBlockPacketization _blockPacketization;
    byte* _buffer;
    MessageHandler& _handler;
};

#endif // !__TCP_CLIENT_H__