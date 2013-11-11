#pragma once
#include "boost\noncopyable.hpp"
#include "network_mgr.h"
#include "packet.h"

class ClientNet : public boost::noncopyable
{
public:
    ClientNet();
    ~ClientNet();

    static ClientNet& Instance()
    {
        static ClientNet inst;
        return inst;
    }

public:
    bool Connect(LPCWSTR IPAddress, unsigned int port);
    void Disconnect();

    template <typename T>
    void Send(uint32 opcode, const T& message)
    {
        assert(m_connected);
        if (m_connected)
        {
            size_t messageSize = message.ByteSize();
            size_t packetSize = ServerPacket::HEADER_LENGTH + messageSize;

            ByteBuffer packet_buffer;
            packet_buffer << packetSize;
            packet_buffer << opcode;

            byte* message_data = new byte[messageSize];
            message.SerializeToArray(message_data, messageSize);
            packet_buffer.append(message_data, messageSize);

            m_net.Send(packet_buffer.buffer(), packet_buffer.size());

            if (message_data)
                delete []message_data;
        }
    }

private:
    CNetworkMgr m_net;
    bool        m_connected;
};

#define NET ClientNet::Instance()
