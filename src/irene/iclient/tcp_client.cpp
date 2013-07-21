#include "stdafx.h"
#include "tcp_client.h"
#include "../common/protocol/demo1.pb.h"

CTcpClient::CTcpClient(boost::asio::io_service& io_service)
    : io_service_(io_service)
    , socket_(io_service)
    , signals_(io_service, SIGINT, SIGTERM)
{
    signals_.async_wait(std::bind(&CTcpClient::Close, this));
}

CTcpClient::~CTcpClient()
{

}

int CTcpClient::Send( const char* pBuf, unsigned int len )
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

void CTcpClient::Connect(tcp::resolver::iterator& endpoint_iterator)
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

    Demo::demo1 demoMsg;
    demoMsg.set_demo_str("123");
    demoMsg.set_demo_int(123);
    std::string strMsg = demoMsg.SerializePartialAsString();
//     
//     struct ServerPacket pack;
//     pack.len = strMsg.size();
//     pack.opcode = 0;
    struct XPacket
    {
        size_t len;
        uint32_t opcode;
    }pack;
    pack.len = strMsg.size()  + 8;
    pack.opcode = 0;

    Send((const char*)&pack, 8);
    Send(strMsg.c_str(), strMsg.size());

    std::shared_ptr<std::thread> t(
        new std::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
    io_service_.run();
    t->join();
}

void CTcpClient::do_write(const char* pBuf, unsigned int len)
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
        if (!msg_queue_.empty())
        {
            boost::asio::async_write(
                socket_,
                boost::asio::buffer(msg_queue_.front().first, msg_queue_.front().second),
                boost::bind(&CTcpClient::handle_write, this, boost::asio::placeholders::error));
        }
        msg_queue_.pop_front();
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

