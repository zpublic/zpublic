#include "tcp_client.h"
#include "message_notification.h"

TcpClient::TcpClient(MessageHandler& handler)
    : _blockPacketization(std::bind(&TcpClient::finishedPacketCallback, this, std::placeholders::_1)),
    _buffer(new byte[MAX_RECV_LEN]), _handler(handler), _socket(nullptr), _reactor(nullptr), _reactorThread(nullptr), _isConnected(false)
{

}

TcpClient::~TcpClient()
{
    resetNetwork();
}

bool TcpClient::connect(Poco::Net::SocketAddress& address, const Poco::Timespan& timeout/* = Poco::Timespan(50000)*/)
{
    try
    {
        if (connected())
        {
            this->close();
        }

        _socket = new Poco::Net::StreamSocket;
        //_socket->setBlocking(false);

        _reactor = new Poco::Net::SocketReactor;
        _reactor->addEventHandler(*_socket, Poco::Observer<TcpClient, Poco::Net::WritableNotification>(*this, &TcpClient::onWritable));
        _reactor->addEventHandler(*_socket, Poco::Observer<TcpClient, Poco::Net::ReadableNotification>(*this, &TcpClient::onReadable));
        _reactor->addEventHandler(*_socket, Poco::Observer<TcpClient, Poco::Net::IdleNotification>(*this, &TcpClient::onIdle));
        _reactor->addEventHandler(*_socket, Poco::Observer<TcpClient, Poco::Net::ShutdownNotification>(*this, &TcpClient::onShutdown));
        _reactorThread = new Poco::Thread("default_reactor");
        _reactorThread->start(*_reactor);

        _socket->connect(address);
        _handler.onConnected();

        _isConnected = true;
    }
    catch (Poco::TimeoutException& e)
    {
        std::cout << e.displayText() << std::endl;
        return false;
    }
    catch (Poco::Exception& e)
    {
        std::cout << e.displayText() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cout << "tcp client unknown exception." << std::endl;
        return false;
    }

    return true;
}


void TcpClient::close()
{
    if (_isConnected == false) return;
    resetNetwork();
    _isConnected = false;
}

void TcpClient::sendMessage(const BasicStreamPtr& stream)
{
    bool writeable = _socket->poll(0, Poco::Net::Socket::SelectMode::SELECT_WRITE);
    if (writeable)
    {
        _socket->sendBytes((const void*)stream->b.begin(), stream->b.size());
    }
}

void TcpClient::sendMessage(int16 opcode, const byte* buff, size_t size)
{
    BasicStreamPtr streamPtr(new BasicStream());

    streamPtr->write((int32)0); //长度预留
    streamPtr->write(opcode);   //操作码
    // ...
    // TODO: 包压缩和加密标志预留

    streamPtr->append((const byte*)buff, size);
    streamPtr->rewriteSize(streamPtr->b.size(), streamPtr->b.begin());

    sendMessage(streamPtr);
}

void TcpClient::sendMessage(uint16 opcode, NetworkMessage& message)
{
    BasicStreamPtr streamPtr(new BasicStream());
    streamPtr->write((int32)0);
    streamPtr->write(opcode);
    // ...
    // TODO: 包压缩和加密标志预留

    streamPtr->resize(NetworkParam::kHeaderLength + message.byteSize());
    message.encode((byte*)streamPtr->b.begin() + NetworkParam::kHeaderLength, message.byteSize());
    streamPtr->rewriteSize(streamPtr->b.size(), streamPtr->b.begin());

    sendMessage(streamPtr);
}

void TcpClient::onWritable(Poco::Net::WritableNotification* notification)
{
}

void TcpClient::onReadable(Poco::Net::ReadableNotification* notification)
{
    int bytes_transferred = _socket->receiveBytes(_buffer, MAX_RECV_LEN, 0);
    //printf("received %d bytes.", bytes_transferred);
    if (bytes_transferred == 0)
    {
        this->close();
        return;
    }

    if (_blockPacketization.appendBlock(_buffer, bytes_transferred) == false)
    {
        this->close();
        return;
    }
}

void TcpClient::onShutdown(Poco::Net::ShutdownNotification* notification)
{
    _handler.onShutdown();
}

void TcpClient::onIdle(Poco::Net::IdleNotification* notification)
{
    Poco::Thread::sleep(1);
}

void TcpClient::onTimeout(Poco::Net::TimeoutNotification* notification)
{

}

void TcpClient::finishedPacketCallback(BasicStreamPtr& packet)
{
    //构造网络消息包给应用层
    uint16 opcode = 0;
    packet->read(opcode);

    NetworkPacket::Ptr packetPtr(new NetworkPacket);
    packetPtr->opcode = opcode;
    packetPtr->message = NetworkPacket::PDU(packet->b.begin() + NetworkParam::kHeaderLength, packet->b.end());
    _handler.onMessage(opcode, packetPtr);
}

void TcpClient::resetNetwork()
{
    if (_reactor != nullptr)
    {
        _reactor->stop();
    }

    if (_reactorThread != nullptr && _reactorThread->isRunning())
    {
        _reactorThread->join();
    }

    if (_socket != nullptr)
    {
        _socket->close();
    }

    SAFE_DELETE(_socket);
    SAFE_DELETE(_reactor);
    SAFE_DELETE(_reactorThread);
}