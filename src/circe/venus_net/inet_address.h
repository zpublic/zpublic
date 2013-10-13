#ifndef INET_ADDRESS_H_
#define INET_ADDRESS_H_

#include <string>
#include <cstdint>

class InetAddress
{
public:
    /// Constructs an endpoint with given port number.
    /// Mostly used in TcpServer listening
    explicit InetAddress(uint16_t port)
    {
        _host = "0.0.0.0";
        _port = port;
    }

    /// Constructs an endpoint with given ip and port.
    /// @address - ip should be "1.2.3.4"
    /// @port - host
    InetAddress(const std::string& host, uint16_t port)
    {
        _host = host;
        _port = port;
    }

    std::string host() const
    {
        return _host;
    }

    uint16_t port() const
    {
        return _port;
    }

    std::string toIpHost() const
    {
        char buff[32] = {0};
#ifdef _WIN32
        sprintf_s(buff, 32, "%s:%d", _host.c_str(), _port);
#else
        sprintf(buff, "%s:%d", _host.c_str(), _port);
#endif

        return buff;
    }

private:
    std::string _host;
    uint16_t _port;
};


#endif //INET_ADDRESS_H_