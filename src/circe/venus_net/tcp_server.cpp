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

    //添加捕捉信号
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

    //构造通信地址结构
    tcp::socket& socket = connection->socket();
    std::string remote_address = socket.remote_endpoint().address().to_string();
    uint16_t remote_port = socket.remote_endpoint().port();
    InetAddress peerAddress(remote_address, remote_port);

    //设置链接回调
    connection->setWriteCompletedCallback(_writeCompletedCallback);
    connection->setReadCompletedCallback(_readCompletedCallback);
    connection->setConnectionClosedCallback(_connectionClosedCallback);

    if (_newConnectionCallback)
    {
        _newConnectionCallback(connection, peerAddress);
    }
}