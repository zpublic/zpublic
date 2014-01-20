#include "message_notification.h"
#include "network_common.h"
#include "logger.h"


NetworkNotification::NetworkNotification(ServerConnection* connection, NotificationType notificationType)
    : _serverConnection(connection), _notificationType(notificationType)
{

}

NetworkNotification::~NetworkNotification()
{

}

ServerConnection* NetworkNotification::connection()
{
    return _serverConnection;
}

NotificationType NetworkNotification::notificationType()
{
    return _notificationType;
}



NewConnectionNotification::NewConnectionNotification(ServerConnection* connection)
    : NetworkNotification(connection, NT_NewConnectionNotification)
{
}

MessageNotification::MessageNotification(ServerConnection* connection, NetworkPacket::Ptr& packet)
    : NetworkNotification(connection, NT_MessageNotification), _networkPacket(packet)
{
}

MessageNotification::~MessageNotification()
{
}

const NetworkPacket::Ptr& MessageNotification::packet()
{
    return _networkPacket;
}

CloseNotification::CloseNotification(ServerConnection* connection, const ShutdownReason& reason)
    : NetworkNotification(connection, NT_CloseNotification), _closeReason(reason)
{
}

CloseNotification::~CloseNotification()
{

}

const ShutdownReason& CloseNotification::shutdownReason()
{
    return _closeReason;
}