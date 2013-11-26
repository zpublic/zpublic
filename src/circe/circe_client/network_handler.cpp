#include "stdafx.h"
#include "network_handler.h"
#include "common.h"
#include "network_common.h"
#include "packet.h"
#include "opcodes_handler.h"

void _stdcall CNetworkHandler::OnConnect()
{
    //printf("conn\n");
}

void _stdcall CNetworkHandler::OnClose()
{
    //printf("close\n");
}

void DispatchPacket(const ServerPacketPtr& packet)
{
    OpcodeHandler* handler = OpcodeTable::instance()[packet->opcode];
    if (handler != NULL)
    {
        NetworkMessage network_message;
        network_message.data = packet->message;
        network_message.data_len = packet->len;

        handler->message_handler(network_message);
    }
}

void _stdcall CNetworkHandler::OnReceive( const BYTE* pData, int iLength )
{
    size_t bodyLen = 0;

    ByteBuffer read_buffer(pData, iLength);
    _buffer.append(read_buffer);

    std::vector<ServerPacketPtr> packetList;

    _buffer.set_rpos(0);

    //���ٴ��ڵ���header���ȣ�����bodyΪ�գ�
    while (_buffer.size() >= sizeof(ServerPacket::HEADER_LENGTH))
    {
        size_t packet_len = 0;
        uint32 opcode = 0;

        _buffer >> packet_len;
        _buffer >> opcode;

        //���ݰ����ȴ��������ճ�����Ϊ�Ƿ����ɵ�
        if (packet_len >= MAX_RECV_LEN || read_buffer.size() >= MAX_RECV_LEN || iLength != packet_len)
        {
            assert(false);
            return;
        }

        if (_buffer.size() < packet_len)
        {
            return;
        }
        else if (_buffer.size() >= packet_len)
        {
            ServerPacketPtr packet(new ServerPacket());
            packet->len = packet_len;
            packet->opcode = opcode;

            //ȡ��body����
            bodyLen = packet_len - ServerPacket::HEADER_LENGTH;

            packet->message = bodyLen == 0 ? nullptr : new byte[bodyLen];

            if (bodyLen != 0)
                _buffer.read(packet->message, bodyLen);

            packetList.push_back(packet);
            _buffer.erase(0, packet_len);
            _buffer.set_rpos(0);
            _buffer.set_wpos(0);
        }
    }

    for (size_t i = 0; i < packetList.size(); ++i)
    {
        const ServerPacketPtr& packet = packetList[i];
        DispatchPacket(packet);
    }

    packetList.clear();
}

void _stdcall CNetworkHandler::OnError( int iErrorCode )
{
    //printf("err:%d\n", iErrorCode);
}
