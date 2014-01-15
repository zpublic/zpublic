#ifndef __MESSAGE_NOTIFICATION_H__
#define __MESSAGE_NOTIFICATION_H__

#include "network_common.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"

class ServerConnection;
class MessageNotification : public Poco::Notification
{
public:
    typedef Poco::AutoPtr<MessageNotification> Ptr;
    MessageNotification(ServerConnection* connection, NetworkPacket::Ptr& packet);
    ~MessageNotification();

    ServerConnection* connection();
    const NetworkPacket::Ptr& packet();

private:
    ServerConnection* _serverConnection;
    NetworkPacket::Ptr _networkPacket;
};

#endif // !__MESSAGE_NOTIFICATION_H__
