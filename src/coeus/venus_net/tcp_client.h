#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "common.h"
#include "Poco/Net/SocketConnector.h"

template <typename ServiceHandler>
class TcpClient
{
public:
    bool connect(const Poco::Net::SocketAddress& address, const Poco::Timespan& timeout);
    void connectAsync(const Poco::Net::SocketAddress& address, const std::function<void ()>& callback);

private:
    Poco::Net::SocketConnector<ServiceHandler> _connector;
};

#endif // !__TCP_CLIENT_H__