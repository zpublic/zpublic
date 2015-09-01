#pragma once

///> PUSH 模型服务端 Socket 监听器
class ZLServerSocketListener : public IZLServerSocketListener
{
public:
    virtual emZLSocketHandleResult OnReceive(ZLConnectId connId, int iLength)
    {
        return emZLSocketHandleResultIgnore;
    }

    virtual emZLSocketHandleResult OnSend(ZLConnectId connId, const BYTE* pData, int iLength)
    {
        return emZLSocketHandleResultIgnore;
    }

    virtual emZLSocketHandleResult OnPrepareListen(SOCKET socket)
    {
        return emZLSocketHandleResultIgnore;
    }

    virtual emZLSocketHandleResult OnAccept(ZLConnectId connId, SOCKET socket)
    {
        return emZLSocketHandleResultIgnore;
    }

    virtual void OnShutdown() {}
};

///> PULL 模型服务端 Socket 监听器
class ZLPullServerSocketListener : public ZLServerSocketListener
{
public:
    virtual emZLSocketHandleResult OnReceive(ZLConnectId connId, int iLength) = 0;
    virtual emZLSocketHandleResult OnReceive(ZLConnectId connId, const BYTE* pData, int iLength)
    {
        return emZLSocketHandleResultIgnore;
    }
};
