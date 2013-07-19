#pragma once
#include "protobuf_dispatcher.h"
#include "i_client_operate.h"

typedef std::deque<std::pair<void*, unsigned int> > MessageQueue;

class CTcpClient
    : public IClientOperate
    , private boost::noncopyable
{
public:
    CTcpClient(
        boost::asio::io_service& io_service,
        tcp::resolver::iterator endpoint_iterator);
    ~CTcpClient();

    virtual int Send(void* pBuf, unsigned int len);

    virtual int RegisterMessageCallback(
        const google::protobuf::Descriptor* desc,
        const MessageCallback& callback );

    virtual int UnregisterMessageCallback(
        const google::protobuf::Descriptor* desc );

    void Connect();
    void Close();

private:

    void do_write(void* pBuf, unsigned int len);

    void handle_write(const boost::system::error_code& error);

    void do_close();
    void handle_connect( const boost::system::error_code& error );
private:
    ProtobufDispatcher disp_;
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    MessageQueue msg_queue_;
    boost::asio::signal_set signals_;
};
