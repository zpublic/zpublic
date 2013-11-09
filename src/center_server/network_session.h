#ifndef __SESSION_H__
#define __SESSION_H__

#include <common.h>
#include <network_common.h>
#include <packet.h>
#include <tcp_connection.h>

#define PARSE_NETWORK_MESSAGE(message, request) \
    if (message.data != nullptr) message.parse(request)

class NetworkSession
{
public:
    NetworkSession(const uint64& session_id);
    virtual ~NetworkSession();

public:
    void set_connection_ptr(const TcpConnectionPtr& connection);
    TcpConnectionPtr& connection();
    uint64 session_id() const;

    template <typename T> void send_message(uint32 opcode, const T& message)
    {
        if (_connection != nullptr)
        {
            size_t messageSize = message.ByteSize();
            size_t packetSize = ServerPacket::HEADER_LENGTH + messageSize;

            ByteBuffer packet_buffer;
            packet_buffer << packetSize;
            packet_buffer << opcode;
            
            byte* message_data = new byte[messageSize];
            message.SerializeToArray(message_data, messageSize);
            packet_buffer.append(message_data, messageSize);

            _connection->writeAsync(packet_buffer.buffer(), packet_buffer.size());

            SAFE_DELETE_ARR(message_data);
        }
    }

    void send_error(uint32 error_code)
    {
        Protocol::S2CError error;
        error.set_error_code(error_code);
        send_message<Protocol::S2CError>(Opcodes::S2CError, error);
    }

    void send_error_ex(uint32 error_code, const std::string& error_reason)
    {
        Protocol::S2CErrorEx error_ex;
        error_ex.set_error_code(error_code);
        error_ex.set_error_reason(error_reason);
        send_message<Protocol::S2CErrorEx>(Opcodes::S2CErrorEx, error_ex);
    }

private:
    uint64 _sessionId;
    TcpConnectionPtr _connection;
};

#endif