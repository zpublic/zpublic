#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "common.h"
#include "basic_stream.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketReactor.h"

struct Message
{
public:
    const static uint8 kHeaderLength = 8;
    const static int32 kMaxMessageLength = 10*65535;

    Message(){}
    virtual ~Message(){}

    virtual int32 byteSize() = 0;
    virtual void encode(byte* buffer, size_t size) = 0;
    virtual void decode(const byte* buffer, size_t size) = 0;
};

class TcpConnection : public Poco::Net::TCPServerConnection
{
    static const int MAX_RECV_LEN = 1024 * 4;

public:
    TcpConnection(const Poco::Net::StreamSocket& socket);
    virtual ~TcpConnection();
    void run();

public:
    void sendMessage(const BasicStreamPtr& packet);
	void sendMessage(int16 opcode, const byte* buff, size_t size);
	void sendMessage(uint16 opcode, Message& message);

private:
    enum ShutdownReason
    {
        SR_GRACEFUL_SHUTDOWN,
        SR_EXCETION
    };

    bool onReadable();
    void onShutdown(const ShutdownReason& reason);
    void addPending(const byte* buff, size_t len);
    bool checkMessageLen(size_t len);

private:
    byte* _buffer;
    BasicStreamPtr _pendingStream;
    Poco::Net::StreamSocket& _socket;
    mutable Poco::FastMutex _mutex;
};

#endif // !__TCP_CONNECTION_H__
