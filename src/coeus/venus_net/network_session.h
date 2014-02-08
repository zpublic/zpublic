#ifndef __NETWORK_SESSION_H__
#define __NETWORK_SESSION_H__

#include "common.h"

class ServerConnection;
class NetworkMessage;
class PHPMessage;
class NetworkSession
{
public:
    NetworkSession(ServerConnection* serverConnection);
    virtual ~NetworkSession();

    bool init();
    void destroy();
    uint64 sessionId() const;
	std::string& address() const;

public:
	void close();
    void send_message(uint32 opcode, NetworkMessage& message);
	//void send_php_message(const PHPMessage& message);

private:
    ServerConnection* _serverConnection;
};
 
#endif