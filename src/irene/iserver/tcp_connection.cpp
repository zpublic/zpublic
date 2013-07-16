#include "stdafx.h"
#include <byte_buffer.h>
#include "tcp_connection.h"
#include "common_def.h"
#include "nagle_packet_fragment_codec.h"

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

        ByteBufferPtr read_buffer(new ByteBuffer(_recvBuffer.data(), bytes_transferred));

        if (append_buffer_fragment(read_buffer))
        {
            if (_integrity_packet != NULL)
            {
                uint32_t opcode = _integrity_packet->opcode;
                google::protobuf::Message* message = _integrity_packet->message;

                if (message == NULL)
                    std::cout << "fatal : NULL proto message!" << std::endl;

                if (_readComplectedCallback)
                    _readComplectedCallback(shared_from_this(), opcode, *message, bytes_transferred);
            }
            else
            {
                std::cout << "fatal : NULL Packet!" << std::endl;
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
    if (_state == S_IDLE && buffer->size() < ServerPacket::MIN_HEADER_LENGTH)
    {
        _buffer.append(buffer);
        _state = S_PROCESSING;
        return false;
    }
    else
    {
        _buffer.append(buffer);
    }

    //检查缓冲区数据是否满足packet的最低字节数
    if (_buffer.size() <= sizeof(ServerPacket))
    {
        std::cout << "buffer size not enough the bytesize of ServerPacket(" << sizeof(ServerPacket) << " bytes)." << std::endl;
        return false;
    }

    size_t packet_len = 0;
    _buffer >> packet_len;

    if (packet_len >= MAX_RECV_LEN)
    {
        std::cout << "Warning: Read packet header length " << packet_len << " bytes (which is too large) on peer socket.\n" << std::endl;
        std::cout << "  Maybe is an invalid packet :(" << std::endl;
        reset();
        //close();
        return false;
    }

    if (_buffer.size() < packet_len)
    {
        return false;
    }
    else if (_buffer.size() == packet_len)
    {
        _integrity_packet = (ServerPacket*)(reinterpret_cast<const ServerPacket*>(_buffer.buffer()));
        reset();
        return true;
    }
    else
    {
        std::cout << "invalid packet" << std::endl;
        return false;
    }
}
