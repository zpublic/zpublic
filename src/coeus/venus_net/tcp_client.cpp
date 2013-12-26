#include "tcp_client.h"

bool TcpClient::connect(const Poco::Net::SocketAddress& address, const Poco::Timespan& timeout)
{
    try
    {
        _socket.close();
        _socket.connect(address, timeout);
    }
    catch (Poco::TimeoutException& e)
    {
    	return false;
    }
    catch (Poco::Exception& e)
    {
        return false;
    }

    return true;
}

void TcpClient::connectAsync(const Poco::Net::SocketAddress& address, const std::function<void ()>& callback)
{
    try
    {
        _socket.close();
        _socket.connectNB(address);
    }
    catch (Poco::Exception& e)
    {
    }
}