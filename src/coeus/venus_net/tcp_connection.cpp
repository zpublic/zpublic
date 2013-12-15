#include "tcp_connection.h"
#include "Poco/Util/Application.h"
#include "service_application.h"

TcpConnection::TcpConnection(const Poco::Net::StreamSocket& socket)
    : Poco::Net::TCPServerConnection(socket), _buffer(new byte[MAX_RECV_LEN])
{
    _socket = &this->socket();
    _socket->setBlocking(false);
}

TcpConnection::~TcpConnection()
{
    SAFE_DELETE_ARR(_buffer);
}

void TcpConnection::run()
{
    try
    {            
        for (;;)
        {
            int bytes_transferred = _socket->receiveBytes(_buffer, MAX_RECV_LEN, 0);
            std::cout << "received " << bytes_transferred << " bytes." << std::endl;
            if (bytes_transferred == 0)
            {
                std::cout << "connection lost :(" << std::endl;
                return;
            }
        }
    }
    catch (Poco::Exception& e)
    {
        std::cout << "connection exception : " + e.message() << std::endl;
    }
}
