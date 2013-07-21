#pragma once
#include "protobuf_dispatcher.h"
#include "i_client_operate.h"

typedef std::deque<std::pair<const char*, unsigned int> > MessageQueue;

class CTcpClient
    : public IClientOperate
    , private boost::noncopyable
{
public:
    CTcpClient(boost::asio::io_service& io_service);
    ~CTcpClient();

    virtual int Send(const char* pBuf, unsigned int len);

    virtual int RegisterMessageCallback(
        const google::protobuf::Descriptor* desc,
        const MessageCallback& callback );

    virtual int UnregisterMessageCallback(
        const google::protobuf::Descriptor* desc );

    void Connect(tcp::resolver::iterator& endpoint_iterator);

private:

    void do_write(const char* pBuf, unsigned int len);

    void handle_write(const boost::system::error_code& error);

    void do_close();

    void Close();
private:
    ProtobufDispatcher disp_;
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    MessageQueue msg_queue_;
    boost::asio::signal_set signals_;
};
