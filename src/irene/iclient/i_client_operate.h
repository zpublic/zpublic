#pragma once

class google::protobuf::Descriptor;
typedef std::function<void (google::protobuf::Message* message)> MessageCallback;

class IClientOperate
{
public:
    virtual ~IClientOperate() {}

    virtual int Send(const char* pBuf, unsigned int len) = 0;

    virtual int RegisterMessageCallback(
        const google::protobuf::Descriptor* desc,
        const MessageCallback& callback) = 0;
    virtual int UnregisterMessageCallback(
        const google::protobuf::Descriptor* desc) = 0;
};
