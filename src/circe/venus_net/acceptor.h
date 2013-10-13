#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <boost/noncopyable.hpp>
#include "inet_address.h"
#include "network_common.h"
#include "io_service.h"

using namespace boost::asio;

class Acceptor 
    : private boost::noncopyable
{
    friend class TcpServer;
protected:
    explicit Acceptor(const InetAddress& listenAddress, IOService& io_service, uint32_t threadNums);
    virtual ~Acceptor();

protected:
    std::string host() const;
    uint16_t port() const;
    void listen(int32_t backlog = socket_base::max_connections);
    void startAccept();
    void stopAccept();
    bool listenning() const;

protected:
    void setAcceptedCallback(const AcceptedCallback& cb);

private:
    void accept();
    void acceptHandler(const TcpConnectionPtr& connection);

private:
    bool _listenning;
    uint32_t _threadNums;
    InetAddress _listenAddr;
    AcceptedCallback _acceptedCallback;
    IOService& _io_service;
    boost::asio::strand _strand;
    boost::asio::ip::tcp::acceptor _acceptor;
};

#endif //ACCEPTOR_H_