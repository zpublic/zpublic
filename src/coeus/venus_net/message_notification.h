#ifndef __MESSAGE_NOTIFICATION_H__
#define __MESSAGE_NOTIFICATION_H__

#include "network_common.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"

class ServerConnection;
enum NotificationType
{
    None,
    NT_NewConnectionNotification,
    NT_MessageNotification,
    NT_CloseNotification
};

class NetworkNotification : public Poco::Notification
{
public:
    NetworkNotification(ServerConnection* connection, NotificationType notificationType);
    virtual ~NetworkNotification();

    ServerConnection* connection();
    NotificationType notificationType();

private:
    NotificationType _notificationType;
    ServerConnection* _serverConnection;
};

class NewConnectionNotification : public NetworkNotification
{
public:
    typedef Poco::AutoPtr<NewConnectionNotification> Ptr;
    NewConnectionNotification(ServerConnection* connection);
};

class MessageNotification : public NetworkNotification
{
public:
    typedef Poco::AutoPtr<MessageNotification> Ptr;
    MessageNotification(ServerConnection* connection, NetworkPacket::Ptr& packet);
    virtual ~MessageNotification();

    const NetworkPacket::Ptr& packet();

private:
    NetworkPacket::Ptr _networkPacket;
};

class CloseNotification : public NetworkNotification
{
public:
    typedef Poco::AutoPtr<CloseNotification> Ptr;
    CloseNotification(ServerConnection* connection, const ShutdownReason& reason);
    virtual ~CloseNotification();

    const ShutdownReason& shutdownReason();

private:
    ShutdownReason _closeReason;
};

#endif // !__MESSAGE_NOTIFICATION_H__
