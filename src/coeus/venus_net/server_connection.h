#ifndef __SERVER_SESSION_H__
#define __SERVER_SESSION_H__

#include "common.h"

// ServerConnection 用于隐藏底层Connection的部分接口
// 提供基本方法给上层对连接进行操作

class TcpConnection;
class NetworkMessage;
class ServerConnection
{
public:
    ServerConnection(TcpConnection* connection);
    virtual ~ServerConnection();

public:
    void sendMessage(uint16 opcode, NetworkMessage& message);
    void close();

private:
    TcpConnection* _connection;
};

#endif