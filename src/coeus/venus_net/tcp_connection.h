#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "common.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"

class TcpConnection : public Poco::Net::TCPServerConnection
{
    static const int MAX_RECV_LEN = 1024 * 4;

public:
    TcpConnection(const Poco::Net::StreamSocket& socket);
    virtual ~TcpConnection();
    void run();

private:
    byte* _buffer;
    Poco::Net::StreamSocket& _socket;
};

#endif // !__TCP_CONNECTION_H__
