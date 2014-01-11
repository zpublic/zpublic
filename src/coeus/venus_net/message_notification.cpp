#include "message_notification.h"
#include "network_common.h"
#include "logger.h"

MessageNotification::MessageNotification(ServerConnection* connection, NetworkPacket::Ptr& packet)
    : _serverConnection(connection), _networkPacket(packet)
{
}

MessageNotification::~MessageNotification()
{
}

ServerConnection* MessageNotification::connection()
{
    return _serverConnection;
}

const NetworkPacket::Ptr& MessageNotification::packet()
{
    return _networkPacket;
}
