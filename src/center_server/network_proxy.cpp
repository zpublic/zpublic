#include "network_proxy.h"
#include "tcp_server.h"
#include "game_io_dispatcher.h"
#include "game_session_manager.h"

NetworkProxy::NetworkProxy()
    : _service(nullptr), 
    _event_handler(nullptr)
{
}

NetworkProxy::~NetworkProxy()
{
    SAFE_DELETE(_server);
}

bool NetworkProxy::init(IOService& service, GameIODataEventHandler* event_handler, uint16 listen_port, uint32 io_thread_numbers)
{
    _service = &service;
    _event_handler = event_handler;

    // initialize network
    // create tcp server instance
    _server = new TcpServer(InetAddress(listen_port), *_service, io_thread_numbers);

    // register io data event handler
	_server->registerNewConnectionEvent(
		BIND_EVENT_HANDLER(&NetworkProxy::__internalNewConnectionEvent, this));

    _server->registerDataWriteFinishedEvent(
        BIND_EVENT_HANDLER(&NetworkProxy::__internalDataWriteFinishedEvent, this));

    _server->registerDataReadEvent(
        BIND_EVENT_HANDLER(&NetworkProxy::__internalDataReadEvent, this));

    _server->registerConnectionClosedEvent(
        BIND_EVENT_HANDLER(&NetworkProxy::__internalConnectionClosedEvent, this));

    // start server
    _server->start();

    return true;
}

void NetworkProxy::destroy()
{
    _server->stop();
}

void NetworkProxy::close_connection(const TcpConnectionPtr& connection)
{
    connection->shutdown();
    connection->close();
}

void NetworkProxy::__internalNewConnectionEvent(const TcpConnectionPtr& connection, const NewConnectionEventArgs& args)
{
    debug_log("Enter NetworkProxy::__internalNewConnectionEvent --");

    //create game session
    GameSession* session = GameSessionManager::getInstance().createSession();
    session->init();
    session->set_connection_ptr(connection);
    connection->setUserData(session);

    debug_log(
        "New Session [NativeHandle = %d, SessionId = %ull, Peer = %s", 
        connection->handle(), 
        session->session_id(), 
        args.peer_address.toIpHost().c_str());

    auto callback = _dispatcher.sessionCreatedEvent;
    if (callback) 
    {
        callback(session);
    }
}

void NetworkProxy::__internalDataWriteFinishedEvent(const TcpConnectionPtr& connection, const DataWriteFinishedEventArgs& args)
{
}

void NetworkProxy::__internalDataReadEvent(const TcpConnectionPtr& connection, const DataReadEventArgs& args)
{
    info_log("received data :");
    info_log("  opcode = %d", args.opcode);
    info_log("  bytes_transferred = %d", args.data_len);

    NetworkMessage network_message;
    network_message.opcode = args.opcode;
    network_message.data = args.data;
    network_message.data_len = args.data_len;

    GameSession* session = connection->getUserData<GameSession>();
    if (session != nullptr)
    {
        auto callback = _dispatcher.sessionMessageEvent;
        if (callback)
        {
            callback(session, &network_message);
        }
    }
}

void NetworkProxy::__internalConnectionClosedEvent(const TcpConnectionPtr& connection, const EventArgs& args)
{
    debug_log("Enter NetworkProxy::__internalConnectionClosedEvent --");

    GameSession* session = connection->getUserData<GameSession>();
    if (session != nullptr)
    {
        //call closed event
        auto callback = _dispatcher.sessionClosingEvent;
        if (callback)
        {
            callback(session);
        }
    }
}

void NetworkProxy::registerSessionCreatedHandler(const SessionCreated& event)
{
    _dispatcher.sessionCreatedEvent = event;
}

void NetworkProxy::registerSessionMessageHandler(const SessionMessage& event)
{
    _dispatcher.sessionMessageEvent = event;
}

void NetworkProxy::registerConnectionClosingHandler(const SessionClosing& event)
{
    _dispatcher.sessionClosingEvent = event;
}