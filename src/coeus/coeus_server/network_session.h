#ifndef __SESSION_H__
#define __SESSION_H__

#include "venus_net/common.h"

class ServerConnection;
class NetworkSession
{
public:
    NetworkSession(ServerConnection* serverConnection);
    virtual ~NetworkSession();

    bool init();
    void destroy();

public:
	void close();
//     void send_message(uint32 opcode, const NetworkMessage& message)
//     {
//         _serverConnection->sendMessage(opcode, message);
//     }

    void send_error(uint32 errorCode);
    void send_error(uint32 errorCode, const std::string& reason);

private:
    ServerConnection* _serverConnection;
};

#endif