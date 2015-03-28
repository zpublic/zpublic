#pragma once

///> 通信服务端组件接口
class IZLServerSocket
{
public:
    /**
    * @brief     启动通信组件
    * @param[in] pszBindAddress     监听地址
    * @param[in] usPort             监听端口
    * @return    TRUE 成功 FALSE 失败（可通过 GetLastError() 获取错误代码）
    */
    virtual BOOL Start(LPCWSTR pszBindAddress, USHORT usPort) = 0;

    /**
    * @brief     关闭通信组件
    * @return    TRUE 成功 FALSE 失败（可通过 GetLastError() 获取错误代码）
    */
    virtual BOOL Stop() = 0;

    /**
    * @brief     发送数据
    * @param[in] connId     连接ID
    * @param[in] pBuffer    发送数据缓冲区
    * @param[in] iLength    发送数据长度
    * @return    TRUE 成功 FALSE 失败（可通过 GetLastError() 获取错误代码）
    */
    virtual BOOL Send(ZLConnectId connId, const BYTE* pBuffer, int iLength) = 0;

    /**
    * @brief     断开连接
    * @param[in] connId     连接ID
    * @param[in] bForce     是否强制断开连接
    * @return    TRUE 成功 FALSE 失败（可通过 GetLastError() 获取错误代码）
    */
    virtual BOOL Disconnect(ZLConnectId connId, BOOL bForce = TRUE) = 0;

public:
    virtual BOOL HasStarted()                                                               = 0;
    virtual emZLServiceState GetState()                                                     = 0;
    virtual emZLServerError GetLastError()                                                  = 0;
    virtual BOOL GetListenAddress(CStringW& strAddress, USHORT& usPort)                     = 0;
    virtual BOOL GetClientAddress(ZLConnectId connId, CStringW& strAddress, USHORT& usPort) = 0;

    ///> 设置工作线程数量（通常为2*CPU+2）
    virtual void SetWorkerThreadCount(DWORD dwWorkerThreadCount)        = 0;
    ///> 设置Accept预投递socket数量（通常为工作线程1-2倍）
    virtual void SetAcceptSocketCount(DWORD dwAcceptSocketCount)        = 0;
    ///> 设置通信缓冲区大小（根据通信包大小调整，通常为1024的倍数）
    virtual void SetSocketBufferSize(DWORD dwSocketBufferSize)          = 0;
    ///> 设置监听socket的等待队列大小（根据并发连接数量调整）
    virtual void SetSocketListenQueue(DWORD dwSocketListenQueue)        = 0;
    ///> 设置socket缓冲池的大小（通常为平均并发连接熟练1/3-1/2）
    virtual void SetFreeSocketObjPool(DWORD dwFreeSocketObjPool)        = 0;
    ///> 设置内存块缓存池的大小（通常为socket缓存池的2-3倍）
    virtual void SetFreeBufferObjPool(DWORD dwFreeBufferObjPool)        = 0;
    ///> 设置socket缓存池回收阀值（通常为socket缓存池的3倍）
    virtual void SetFreeSocketObjHold(DWORD dwFreeSocketObjPool)        = 0;
    ///> 设置内存块缓存池回收阀值（通常为内存块缓存池大小的3倍）
    virtual void SetFreeBufferObjHold(DWORD dwFreeBufferObjHold)        = 0;
    ///> 设置心跳包间隔
    virtual void SetKeepAliveTime(DWORD dwKeepAliveTime)                = 0;
    ///> 设置心跳包确认检测间隔
    virtual void SetKeepAliveInterval(DWORD dwKeepAliveInterval)        = 0;
    ///> 设置关闭服务前等待连接关闭的最长时限
    virtual void SetMaxShutdownWaitTime(DWORD dwMaxShutdownWaitTime)    = 0;

    virtual DWORD GetWorkThreadCount()                                  = 0;
    virtual DWORD GetAcceptSocketCount()                                = 0;
    virtual DWORD GetSocketBufferSize()                                 = 0;
    virtual DWORD GetSocketListenQueue()                                = 0;
    virtual DWORD GetFreeSocketObjPool()                                = 0;
    virtual DWORD GetFreeBufferObjPool()                                = 0;
    virtual DWORD GetFreeSocketObjHold()                                = 0;
    virtual DWORD GetFreeBufferObjHold()                                = 0;
    virtual DWORD GetKeepAliveTime()                                    = 0;
    virtual DWORD GetKeepAliveInterval()                                = 0;
    virtual DWORD GetMaxShutdownWaitTime()                              = 0;

public:
    virtual ~IZLServerSocket() {}
};
