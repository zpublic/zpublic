#include "message_notification.h"
#include "network_common.h"
#include "logger.h"


NetworkNotification::NetworkNotification(ServerConnection* connection)
    : _serverConnection(connection), _notificationType(NotificationType::None)
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

MessageNotification::MessageNotification(ServerConnection* connection, NetworkPacket::Ptr& packet)
    : NetworkNotification(connection), _networkPacket(packet)
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
    : NetworkNotification(connection), _closeReason(reason)
{
}

CloseNotification::~CloseNotification()
{

}

const ShutdownReason& CloseNotification::shutdownReason()
{
    return _closeReason;
}