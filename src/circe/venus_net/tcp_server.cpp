#include "stdafx.h"
#include "inet_address.h"
#include "tcp_server.h"
#include "network_common.h"
#include "tcp_connection.h"

TcpServer::TcpServer(const InetAddress& listenAddress, IOService& service, uint32_t threadNums)
    : _io_service(service),
    _acceptor(listenAddress, service, threadNums),
    _started(false),
    _signals(service.service())
{
    std::cout << "Server starting ..." << std::endl;

    //Ìí¼Ó²¶×½ÐÅºÅ
    _signals.add(SIGINT);
    _signals.add(SIGTERM);

#if defined(SIGQUIT)
    _signals.add(SIGQUIT);
#endif
    _signals.async_wait(std::bind(&TcpServer::stop, this));

    _acceptor.setAcceptedCallback(
        std::bind(&TcpServer::newConnectionCallback, this, std::placeholders::_1)
        );
}

TcpServer::~TcpServer()
{
    _signals.clear();
}

void TcpServer::start()
{
    if (!_started)
    {
        _started = true;
    }
    else
    {
        stop();
    }

    _acceptor.listen();
    _acceptor.startAccept();
}

void TcpServer::stop()
{
    //stop acceptor
    _acceptor.stopAccept();

    //close all alived connections
    // ...
    std::cout << "Server stopped." << std::endl;
}

void TcpServer::setNewConnectionCallback(const NewConnectionCallback& cb)
{
    _newConnectionCallback = cb;
}

void TcpServer::setWriteCompletedCallback(const WriteCompletedCallback& cb)
{
    _writeCompletedCallback = cb;
}

void TcpServer::setReadCompletedCallback(const ReadCompletedCallback& cb)
{
    _readCompletedCallback = cb;
}

void TcpServer::setConnectionClosedCallback(const ConnectionClosedCallback& cb)
{
    _connectionClosedCallback = cb;
}

void TcpServer::newConnectionCallback(const TcpConnectionPtr& connection)
{
    std::cout << "thread id = " << std::this_thread::get_id() << std::endl;

    tcp::socket& socket = connection->socket();
    std::string remote_address = socket.remote_endpoint().address().to_string();
    uint16 remote_port = socket.remote_endpoint().port();
    InetAddress peerAddress(remote_address, remote_port);

    //callbacks
    connection->setWriteCompletedCallback(_writeCompletedCallback);
    connection->setReadCompletedCallback(_readCompletedCallback);
    connection->setConnectionClosedCallback(_connectionClosedCallback);

    if (_newConnectionCallback)
    {
        _newConnectionCallback(connection, peerAddress);
    }
}