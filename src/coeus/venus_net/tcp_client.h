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

class TcpClient
{
    static const int MAX_RECV_LEN = 1024 * 4;

public:
    TcpClient();
    virtual ~TcpClient();

    bool connect(Poco::Net::SocketAddress& address, const Poco::Timespan& timeout = Poco::Timespan(50000));
    void close();
    void sendMessage(const BasicStreamPtr& stream);
    void sendMessage(int16 opcode, const byte* buff, size_t size);
    void sendMessage(uint16 opcode, NetworkMessage& message);

private:
    void onWritable(const Poco::AutoPtr<Poco::Net::WritableNotification>& notification);
    void onReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& notification);
    void onShutdown(const Poco::AutoPtr<Poco::Net::ShutdownNotification>& notification);
    void onMessage(const BasicStreamPtr& packet);

private:
    Poco::Net::SocketReactor _reactor;
    Poco::Net::StreamSocket _socket;
    MessageBlockPacketization _blockPacketization;
    byte* _buffer;
    bool _reconnectFlag;
};

#endif // !__TCP_CLIENT_H__