#ifndef NAGLE_CODEC_H_
#define NAGLE_CODEC_H_

#include <byte_buffer.h>
#include <packet.h>
#include "common_def.h"

class NaglePacketFragmentCodec
{
    enum CODEC_STATE { S_IDLE, S_PROCESSING } _state;

public:
    NaglePacketFragmentCodec()
        : _state(S_IDLE), _packet(NULL)
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

        if (_buffer.size() < packet_len)
        {
            return false;
        }
        else if (_buffer.size() == packet_len)
        {
            _packet = (ServerPacket*)(reinterpret_cast<const ServerPacket*>(_buffer.buffer()));
            _state = S_IDLE;
            _buffer.clear();
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
    ByteBuffer _buffer;
    ServerPacket* _packet;
};

#endif