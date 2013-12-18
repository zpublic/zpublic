#include "tcp_connection.h"
#include "Poco/Util/Application.h"
#include "service_application.h"
#include "logger.h"

TcpConnection::TcpConnection(const Poco::Net::StreamSocket& socket)
    : Poco::Net::TCPServerConnection(socket),
    _socket(const_cast<Poco::Net::StreamSocket&>(socket)),
    _buffer(new byte[MAX_RECV_LEN])
{
    _socket.setBlocking(false);
    _socket.setReuseAddress(true);
    _socket.setReusePort(true);
    _socket.setKeepAlive(true);
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
        Poco::Net::SocketAddress address = socket().address();
        std::string peer = address.toString();
        debug_log("connection established. peer = %s", peer.c_str());

        //sendMessage(123, (const byte*)"hello", 5);
        for (;;)
        {
			bool readable = socket().poll(0, Poco::Net::Socket::SelectMode::SELECT_READ);
			if (readable == true)
            {
                std::memset(_buffer, 0, MAX_RECV_LEN);
                int bytes_transferred = socket().receiveBytes(_buffer, MAX_RECV_LEN);
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
    }
    catch (...)
    {
        error_log("unknown exception.");
    }
}


void TcpConnection::sendMessage(const BasicStreamPtr& stream)
{
    bool writeable = _socket.poll(0, Poco::Net::Socket::SelectMode::SELECT_WRITE);
    if (writeable)
    {
        _socket.sendBytes(stream->b.begin(), stream->b.size(), 0);
    }
}

void TcpConnection::sendMessage(int16 opcode, const byte* buff, size_t size)
{
    BasicStreamPtr streamPtr(new BasicStream());

    streamPtr->write((int32)0); //³¤¶ÈÔ¤Áô
    streamPtr->write(opcode);   //²Ù×÷Âë

    streamPtr->append((const byte*)buff, size);
    streamPtr->rewriteSize(streamPtr->b.size(), streamPtr->b.begin());

    sendMessage(streamPtr);
}

void TcpConnection::sendMessage(uint16 opcode, Message& message)
{
    BasicStreamPtr streamPtr(new BasicStream());
    streamPtr->write((int32)0);
    streamPtr->write(opcode);

    streamPtr->resize(Message::kHeaderLength + message.byteSize());
    message.encode((byte*)streamPtr->b.begin() + Message::kHeaderLength, message.byteSize());
    streamPtr->rewriteSize(streamPtr->b.size(), streamPtr->b.begin());

    sendMessage(streamPtr);
}