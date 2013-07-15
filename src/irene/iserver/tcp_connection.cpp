#include "stdafx.h"
#include "tcp_connection.h"
#include "common_def.h"

TcpConnection::TcpConnection(IOService& io_service)
    : _io_service(io_service),
    _strand(io_service.service()),
    _socket(io_service.service())
{
}

TcpConnection::~TcpConnection()
{
    close();
    std::cout << "connection destroyed." << std::endl;
}

void TcpConnection::write(const byte* data, size_t size)
{
    if (!data)
    {
        std::cout << "empty data." << std::endl;
        return;
    }

    boost::asio::async_write(
        _socket,
        boost::asio::buffer(data, size),
        _strand.wrap(
            boost::bind(
                &TcpConnection::handleWrite, 
                shared_from_this(), 
                boost::asio::placeholders::error, 
                boost::asio::placeholders::bytes_transferred
            )
        )
    );
}

void TcpConnection::read()
{
    _socket.async_read_some(
        boost::asio::buffer(_recvBuffer),
        _strand.wrap(
            boost::bind(
                &TcpConnection::handleRead, 
                shared_from_this(), 
                boost::asio::placeholders::error, 
                boost::asio::placeholders::bytes_transferred
            )
        )
    );
}

void TcpConnection::shutdown()
{
    _socket.shutdown(boost::asio::socket_base::shutdown_both);
}

void TcpConnection::close()
{
    _socket.close();
}


tcp::socket& TcpConnection::socket()
{
    return _socket;
}

bool TcpConnection::isOpen()
{
    return _socket.is_open();
}

void TcpConnection::setWriteCompletedCallback(const WriteCompletedCallback& cb)
{
    _writeCompletedCallback = cb;
}

void TcpConnection::setReadCompletedCallback(const ReadCompletedCallback& cb)
{
    _readComplectedCallback = cb;
}

void TcpConnection::setConnectionClosedCallback(const ConnectionClosedCallback& cb)
{
    _connectionClosedCallback = cb;
}

void TcpConnection::handleWrite(
    const boost::system::error_code& error, // Result of operation.
    std::size_t bytes_transferred           // Number of bytes sent.
)
{
    if (error)
    {
        onError(error);
    }
    else
    {
        std::cout << "bytes_transferred = " << bytes_transferred << std::endl;
        if (_writeCompletedCallback)
        {
            _writeCompletedCallback(shared_from_this(), bytes_transferred);
        }
        else
        {
            std::cout << "write complected." << std::endl;
        }
    }
}

void TcpConnection::handleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (error)
    {
        onError(error);
    }
    else
    {
        if (bytes_transferred == 0)
        {
            std::cout << "oops, connection lost :(" << std::endl;
            return;
        }

        this->read();
        if (_readComplectedCallback)
        {
            _readComplectedCallback(shared_from_this(), 1, bytes_transferred);
        }
    }
}

void TcpConnection::onError(const boost::system::error_code& error)
{
    std::cout << "An error occured, code = " << error.value() << ", message = " << error.message() << std::endl;
    
    shutdown();
    switch (error.value())
    {
        case boost::asio::error::bad_descriptor:
        case boost::asio::error::eof:
        case boost::asio::error::operation_aborted:
        case boost::asio::error::connection_reset:
        {
            if (_connectionClosedCallback)
            {
                _connectionClosedCallback(shared_from_this());
            }
            break;
        }
    }
}