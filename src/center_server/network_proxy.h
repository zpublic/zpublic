#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#include <network_common.h>
#include <singleton.h>
#include <io_service.h>
#include "session_io.h"

struct SessionPair
{

};

class TcpServer;
class GameIODataEventHandler;
class NetworkProxy
    : public Venus::Singleton<NetworkProxy>
{
public:
    NetworkProxy();
    virtual ~NetworkProxy();

public:
    // 初始化数据
    bool init(IOService& service, GameIODataEventHandler* event_handler, uint16 listen_port, uint32 io_thread_numbers);

    // 释放数据
    void destroy();

    // 主动关闭一个上层连接
    // 注：通过主动调用该接口关闭连接时，会触发到上层的ConnectionClosed事件，释放游戏资源即可
    void close_connection(const TcpConnectionPtr& connection);

public:
    // IO事件注册
    void registerSessionCreatedHandler(const SessionCreated& event);
    void registerSessionMessageHandler(const SessionMessage& event);
    void registerConnectionClosingHandler(const SessionClosing& event);

private:
    // 内部事件
    // 收到数据之前先由NetworkProxy接管，处理一些连接的管理以及回收工作再回调到上层
    void __internalNewConnectionEvent(const TcpConnectionPtr& connection, const NewConnectionEventArgs& args);
    void __internalDataWriteFinishedEvent(const TcpConnectionPtr& connection, const DataWriteFinishedEventArgs& args);
	void __internalDataReadEvent(const TcpConnectionPtr& connection, const DataReadEventArgs& args);
	void __internalConnectionClosedEvent(const TcpConnectionPtr& connection, const EventArgs& args);

private:
    IODataDispatcher _dispatcher;
    GameIODataEventHandler* _event_handler;
    IOService* _service;
    TcpServer* _server;

    // 管理所有连接
    // - first  : handle
    // - second : std::pair<TcpConnectionPtr*, GameSession*>
	adap_map<uint32, std::pair<const TcpConnectionPtr*, GameSession*>> _connections;
};

#endif