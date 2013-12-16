#ifndef __CONNECTION_FACTORY_H__
#define __CONNECTION_FACTORY_H__

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"
#include "tcp_connection.h"

class ConnectionFactory : public Poco::Net::TCPServerConnectionFactory
{
public:
    Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket)
    {
        return new TcpConnection(socket);
    }
};

#endif // !__CONNECTION_FACTORY_H__
