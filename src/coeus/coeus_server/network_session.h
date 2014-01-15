#ifndef __SESSION_H__
#define __SESSION_H__

#include "venus_net/common.h"
#include "venus_net/server_connection.h"

class NetworkMessage;
class NetworkSession
{
public:
    NetworkSession(ServerConnection* serverConnection);
    virtual ~NetworkSession();

    bool init();
    void destroy();
    inline uint32 sessionId() const
    {
        return _serverConnection->sequence();
    }

public:
	void close();
    void send_message(uint32 opcode, NetworkMessage& message);
    void send_error(uint32 errorCode);
    void send_error(uint32 errorCode, const std::string& reason);

private:
    ServerConnection* _serverConnection;
};
 
#endif