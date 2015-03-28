#pragma once

class IZLServerSocketListener : public IZLSocketListener
{
public:
    virtual emZLSocketHandleResult OnPrepareListen(SOCKET socket) = 0;
    virtual emZLSocketHandleResult OnAccept(ZLConnectId connId, SOCKET socket) = 0;
    virtual void OnShutdown() = 0;
};
