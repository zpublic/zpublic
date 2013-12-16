#include "tcp_connection.h"
#include "Poco/Util/Application.h"
#include "service_application.h"

TcpConnection::TcpConnection(const Poco::Net::StreamSocket& socket)
    : Poco::Net::TCPServerConnection(socket),
    _socket(const_cast<Poco::Net::StreamSocket&>(socket)),
    _buffer(new byte[MAX_RECV_LEN])
{
    _socket.setBlocking(false);
    //_socket.setReceiveTimeout(100000);
    //_socket->setReuseAddress(true);
}

TcpConnection::~TcpConnection()
{
    SAFE_DELETE_ARR(_buffer);
    std::cout << "connection destroyed." << std::endl;
}

void TcpConnection::run()
{
    try
    {
        for (;;)
        {
			//enum SelectMode
			//{
			//	SELECT_READ  = 1,
			//	SELECT_WRITE = 2,
			//	SELECT_ERROR = 4
			//};
			bool readable = _socket.poll(0, Poco::Net::Socket::SelectMode::SELECT_READ);
			//int readable_bytes = _socket.available();
			if (readable == true /*&& readable_bytes > 0*/)
            {
                std::memset(_buffer, 0, MAX_RECV_LEN);
                int bytes_transferred = _socket.receiveBytes(_buffer, MAX_RECV_LEN);
                std::cout << "received " << bytes_transferred << " bytes." << std::endl;

                if (bytes_transferred == 0)
                {
                    return;
                }
            }
        }
    }
    catch (Poco::Exception& e)
    {
        //std::cerr << "connection exception : " + e.displayText() << std::endl;
        //return;
    }
}

void TcpConnection::closeConnection()
{
}