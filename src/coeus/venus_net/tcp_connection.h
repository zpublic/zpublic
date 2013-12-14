#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"

class TcpConnection : public Poco::Net::TCPServerConnection
{
public:
    TcpConnection(const Poco::Net::StreamSocket& socket)
        : Poco::Net::TCPServerConnection(socket)
    {
        this->socket().setBlocking(false);
    }

    void run()
    {

    }

};

#endif // !__TCP_CONNECTION_H__
