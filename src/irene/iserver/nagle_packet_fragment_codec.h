#ifndef NAGLE_CODEC_H_
#define NAGLE_CODEC_H_

#include <byte_buffer.h>
#include <packet.h>
#include "tcp_connection.h"

class NaglePacketFragmentCodec
{
    enum CODEC_STATE { S_IDLE, S_PROCESSING } _state;

public:
    NaglePacketFragmentCodec(const TcpConnectionPtr& connection)
        : _state(S_IDLE), _packet(NULL), _connection(connection)
    {
    }

    ~NaglePacketFragmentCodec()
    {
    }

public:
    bool append(const ByteBufferPtr& buffer)
    {
        if (_state == S_IDLE && buffer->size() < ServerPacket::MIN_HEADER_LENGTH)
        {
            _buffer.append(buffer);
            _state = S_PROCESSING;
            return false;
        }
        else
        {
            _buffer.append(buffer);
        }

        //检查缓冲区数据是否满足packet的最低字节数
        if (_buffer.size() <= sizeof(ServerPacket))
        {
            std::cout << "buffer size not enough the bytesize of ServerPacket(" << sizeof(ServerPacket) << " bytes)." << std::endl;
            return false;
        }

        size_t packet_len = 0;
        _buffer >> packet_len;

        if (packet_len >= MAX_RECV_LEN)
        {
            std::cout << "Warning: Read packet header length flag " << packet_len << " bytes (which is too large) to a peer socket.\n" << std::endl;
            std::cout << "  Maybe is an invalid packet :(" << std::endl;
            reset();
            _connection->close();
            return false;
        }

        if (_buffer.size() < packet_len)
        {
            return false;
        }
        else if (_buffer.size() == packet_len)
        {
            _packet = (ServerPacket*)(reinterpret_cast<const ServerPacket*>(_buffer.buffer()));
            reset();
            return true;
        }
        else
        {
            std::cout << "invalid packet" << std::endl;
            return false;
        }
    }

    const ServerPacket* const packet()
    {
        return _packet;
    }

private:
    void reset()
    {
        _state = S_IDLE;
        _buffer.clear();
    }

private:
    ByteBuffer _buffer;
    ServerPacket* _packet;
    TcpConnectionPtr _connection;
};

#endif