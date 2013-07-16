#pragma once

class google::protobuf::Descriptor;

class IClientOperate
{
public:
    virtual ~IClientOperate() {}

    virtual int Send(void* pBuf, unsigned int len) = 0;

    virtual int RegisterMessageCallback(
        const google::protobuf::Descriptor* desc,
        const MessageCallback& callback) = 0;
    virtual int UnregisterMessageCallback(
        const google::protobuf::Descriptor* desc) = 0;
};
