#pragma once

///> PUSH 模型客户端 Socket 监听器
class ZLClientSocketListener : public IZLClientSocketListener
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

    virtual emZLSocketHandleResult OnPrepareConnect(ZLConnectId connId, SOCKET socket)
    {
        return emZLSocketHandleResultIgnore;
    }

    virtual emZLSocketHandleResult OnConnect(ZLConnectId connId)
    {
        return emZLSocketHandleResultIgnore;
    }
};

///> PULL 模型客户端 Socket 监听器
class ZLPullClientSocketListener : public ZLClientSocketListener
{
public:
    virtual emZLSocketHandleResult OnReceive(ZLConnectId connId, int iLength) = 0;
    virtual emZLSocketHandleResult OnReceive(ZLConnectId connId, const BYTE* pData, int iLength)
    {
        return emZLSocketHandleResultIgnore;
    }
};
