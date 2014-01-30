#ifndef __SERVER_SESSION_H__
#define __SERVER_SESSION_H__

#include "network_common.h"
#include "tcp_connection.h"

// ServerConnection 用于隐藏底层Connection的部分接口
// 提供基本方法给上层对连接进行操作

class NetworkMessage;
class ServerConnection
{
public:
    ServerConnection(TcpConnection* connection);
    virtual ~ServerConnection();

public:
    void sendMessage(uint16 opcode, NetworkMessage& message);
	std::string& address() const;
    void close(const ShutdownReason& reason = SR_SERVICE_CLOSE_INITIATIVE);
    uint64 sequence() const;
    void notifyRelease();

private:
    TcpConnection* _connection;
};

#endif