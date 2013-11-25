#include "network_session.h"
#include "network_proxy.h"

NetworkSession::NetworkSession(const uint64& session_id)
    : _sessionId(session_id),
    _connection(nullptr)
{
}

NetworkSession::~NetworkSession()
{
}

bool NetworkSession::init()
{
    return true;
}

void NetworkSession::destroy()
{

}

void NetworkSession::set_connection_ptr(const TcpConnectionPtr& connection)
{
    _connection = connection;
}

TcpConnectionPtr& NetworkSession::connection()
{
    return _connection;
}

void NetworkSession::closeSession()
{
	NetworkProxy::getInstance().close_connection(_connection);
}

uint64 NetworkSession::session_id() const
{
    return _sessionId;
}
