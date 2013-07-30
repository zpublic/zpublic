#include "stdafx.h"
#include "acceptor.h"
#include "tcp_connection.h"
#include "network_common.h"
#include "io_service.h"

Acceptor::Acceptor(const InetAddress& listenAddress, IOService& service, uint32_t threadNums)
    : _io_service(service),
    _acceptor(service.service()),
    _strand(service.service()),
    _listenAddr(listenAddress),
    _listenning(false),
    _threadNums(threadNums)
{
    std::cout << "Acceptor starting ..." << std::endl;
    std::cout << "  >> Listen Address = " << _listenAddr.toIpHost() << std::endl;
    std::cout << "  >> I/O-Thread Numbers = " << threadNums << std::endl;

    //绑定地址
    boost::asio::ip::address address;
    address.from_string(_listenAddr.host());
    tcp::endpoint endpoint(address, _listenAddr.port());
    _acceptor.open(tcp::v4());
    _acceptor.bind(endpoint);

    std::cout << "Acceptor started." << std::endl;
}

Acceptor::~Acceptor()
{
}

std::string Acceptor::host() const
{
    return _listenAddr.host();
}

uint16_t Acceptor::port() const
{
    return _listenAddr.port();
}

void Acceptor::listen(int32_t backlog/* = socket_base::max_connections*/)
{
    _acceptor.listen(backlog);
    _listenning = true;

    std::cout << "Listenning ... " << std::endl;
}

void Acceptor::startAccept()
{
    assert(_threadNums != 0);

    //投递一个接受事件
    accept();

    //为IO队列创建线程
    std::vector<ThreadPtr> threads;
    for (std::size_t i = 0; i < _threadNums; ++i)
    {
        ThreadPtr t(
            new std::thread(
                boost::bind(&boost::asio::io_service::run, &_io_service.service())
            )
        );
        threads.push_back(t);
    }

    for(std::size_t i = 0; i < threads.size(); ++i)
    {
        threads[i]->join();
    }
}

void Acceptor::stopAccept()
{
    _io_service.service().stop();
    _acceptor.close();
    std::cout << "Acceptor stopped." << std::endl;
}

bool Acceptor::listenning() const { return _listenning; }

void Acceptor::setAcceptedCallback(const AcceptedCallback& cb)
{
    _acceptedCallback = cb;
}

void Acceptor::accept()
{
    //创建一个新的连接（事后增加连接池，避免开辟内存的开销）
    TcpConnectionPtr new_connection(new TcpConnection(_io_service)); 

    //投递一个accept请求到io队列，并回调到acceptHandler
    _acceptor.async_accept(
        new_connection->socket(),
        _strand.wrap(
            std::bind(&Acceptor::acceptHandler, this, new_connection)
        )
    );
}

void Acceptor::acceptHandler(const TcpConnectionPtr& connection)
{
    accept();
    _acceptedCallback(connection);

    //投递一个读请求
    connection->read();
}