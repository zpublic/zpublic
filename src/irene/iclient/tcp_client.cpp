#include "stdafx.h"
#include "tcp_client.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

CTcpClient::CTcpClient(
    boost::asio::io_service& io_service,
    tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service)
    , socket_(io_service)
    , signals_(io_service, SIGINT, SIGTERM)
{
    boost::system::error_code ec;
    while (true)
    {
        boost::asio::connect(
            socket_,
            endpoint_iterator,
            ec);
        if (!ec)
        {
            break;
        }
        ///> 如果没连上，30秒重试
        ::Sleep(30000);
    }

    signals_.async_wait(std::bind(&CTcpClient::Close, this));
}

CTcpClient::~CTcpClient()
{

}

int CTcpClient::Send( void* pBuf, unsigned int len )
{
    io_service_.post(std::bind(&CTcpClient::do_write, this, pBuf, len));
    return 0;
}

int CTcpClient::RegisterMessageCallback(
    const google::protobuf::Descriptor* desc,
    const MessageCallback& callback )
{
    if (desc)
    {
        disp_.registerMessageCallback(desc, callback);
        return 0;
    }
    return -1;
}

int CTcpClient::UnregisterMessageCallback(
    const google::protobuf::Descriptor* desc)
{
    if (desc)
    {
        disp_.unregisterMessageCallback(desc);
        return 0;
    }
    return -1;
}

void CTcpClient::Close()
{
    io_service_.post(std::bind(&CTcpClient::do_close, this));
}

void CTcpClient::Connect()
{
    std::shared_ptr<std::thread> t(
        new std::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
    io_service_.run();
    t->join();
}

void CTcpClient::do_write(void* pBuf, unsigned int len)
{
    msg_queue_.push_back(std::make_pair(pBuf, len));

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(msg_queue_.front().first, msg_queue_.front().second),
        boost::bind(&CTcpClient::handle_write, this, boost::asio::placeholders::error));
}

void CTcpClient::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {
        msg_queue_.pop_front();
        if (!msg_queue_.empty())
        {
            boost::asio::async_write(
                socket_,
                boost::asio::buffer(msg_queue_.front().first, msg_queue_.front().second),
                boost::bind(&CTcpClient::handle_write, this, boost::asio::placeholders::error));
        }
    }
    else
    {
        do_close();
    }
}

void CTcpClient::do_close()
{
    socket_.close();
}

