#include "stdafx.h"
#include "byte_buffer.h"
#include "tcp_connection.h"
#include "network_common.h"
#include "packet.h"

const static int DEFAULT_CIRCULAR_BUFFER_SIZE = 512;
TcpConnection::TcpConnection(IOService& io_service)
    : _io_service(io_service),
    _strand(io_service.service()),
    _socket(io_service.service())
{
}

TcpConnection::~TcpConnection()
{
    if (isOpen())
        _socket.close();

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
        return onError(error);

    if (bytes_transferred == 0)
    {
        std::cout << "oops, connection lost :(" << std::endl;
        return;
    }
    
    this->read();
    ByteBufferPtr read_buffer(new ByteBuffer(_recvBuffer.data(), bytes_transferred));
    if (append_buffer_fragment(read_buffer))
    {
        for (size_t i = 0; i < _prepare_packet_list.size(); ++i)
        {
            const uint32_t& opcode = _prepare_packet_list[i].opcode;
            const google::protobuf::Message* message = _prepare_packet_list[i].protoMessage();

            std::cout << "Network Message : [opcode = " <<  opcode << "]" << std::endl;

            if (message != NULL && _readComplectedCallback)
            {
                _readComplectedCallback(shared_from_this(), opcode, *message, bytes_transferred);
            }
            else
            {
                std::cout << "Warnning : empty proto message!" << std::endl;
            }
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

bool TcpConnection::append_buffer_fragment(const ByteBufferPtr& buffer)
{
    _buffer.append(*buffer);
    while (_buffer.size() >= sizeof(ServerPacket))
    {
        size_t packet_len = 0;
        _buffer >> packet_len;

        //数据包长度大于最大接收长度视为非法，干掉
        if (packet_len >= MAX_RECV_LEN)
        {
            std::cout << "Warning: Read packet header length " << packet_len << " bytes (which is too large) on peer socket. (Invalid Packet?)" << std::endl;
            shutdown();
            return false;
        }

        if (_buffer.size() < packet_len)
        {
            return false;
        }
        else if (_buffer.size() == packet_len)
        {
            ServerPacket* packet = 
                (ServerPacket*)(reinterpret_cast<const ServerPacket*>(_buffer.buffer()));

            _prepare_packet_list.push_back(*packet);
            _buffer.clear();
        }
        else
        {
            ServerPacket* packet = 
                (ServerPacket*)(reinterpret_cast<const ServerPacket*>(_buffer.buffer()));

            _prepare_packet_list.push_back(*packet);
            _buffer.erase(0, packet_len);
            _buffer.set_rpos(0);
        }
    }

    return (_prepare_packet_list.size() > 0);
}
