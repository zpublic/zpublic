#pragma once
#include "protobuf_dispatcher.h"
#include "i_client_operate.h"

class CTcpClient : public IClientOperate
{
public:
    CTcpClient();
    ~CTcpClient();

    virtual int Send(void* pBuf, unsigned int len);

    virtual int RegisterMessageCallback(
        const google::protobuf::Descriptor* desc,
        const MessageCallback& callback );

    virtual int UnregisterMessageCallback(
        const google::protobuf::Descriptor* desc );


private:
    ProtobufDispatcher m_disp;
};
