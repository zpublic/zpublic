#include "tcp_connection.h"
#include "Poco/Util/Application.h"
#include "service_application.h"

TcpConnection::TcpConnection(const Poco::Net::StreamSocket& socket)
    : Poco::Net::TCPServerConnection(socket),
    _socket(const_cast<Poco::Net::StreamSocket&>(socket)),
    _buffer(new byte[MAX_RECV_LEN])
{
    _socket.setBlocking(false);
    _socket.setReuseAddress(true);
}

TcpConnection::~TcpConnection()
{
    SAFE_DELETE_ARR(_buffer);
    debug_log("connection destroyed.");
}

void TcpConnection::run()
{
    try
    {
		//debug_log("connection established. address = %s", _socket.peerAddress().toString().c_str());
		debug_log(std::string("aaa").c_str());
		std::cout << "ddd" << std::endl;
		printf("xxx");
        for (;;)
        {        
			bool readable = _socket.poll(10000, Poco::Net::Socket::SelectMode::SELECT_READ);
			if (readable == true)
            {
                std::memset(_buffer, 0, MAX_RECV_LEN);
                int bytes_transferred = _socket.receiveBytes(_buffer, MAX_RECV_LEN);
                debug_log("received %d bytes.", bytes_transferred);

                if (bytes_transferred == 0)
                {
                    debug_log("connection graceful shutdown from the peer.");
                    return;
                }
            }
        }
    }
    catch (Poco::Exception& e)
    {
        error_log("connection exception : %s", e.displayText().c_str());
        return;
    }
}