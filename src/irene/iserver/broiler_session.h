#ifndef BROILER_SESSION_H_
#define BROILER_SESSION_H_

#include <irene_common.h>
#include <network_common.h>

class BroilerSession
{
public:
    BroilerSession(const uint64_t& session_id)
        : _sessionId(session_id)
    {
    }

    ~BroilerSession()
    {
    }

public:
    void set_connection_ptr(const TcpConnectionPtr& connection)
    {
        _connection = connection;
    }

    uint64_t session_id() const
    {
        return _sessionId;
    }

    void send_message(uint32_t opcode, const ::google::protobuf::Message& message)
    {
    }

public:
    void login_request_handler(const google::protobuf::Message& message)
    {
        std::cout << "enter login handler, session id = " << _sessionId << std::endl;
    }

private:
    uint64_t _sessionId;
    TcpConnectionPtr _connection;
};

#endif