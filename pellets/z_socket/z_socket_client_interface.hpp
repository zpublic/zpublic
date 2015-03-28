#pragma once

///> 通信客户端组件接口
class IZLClientSocket
{
public:
    /**
    * @brief     启动通信组件
    * @param[in] pszRemoteAddress   服务端地址
    * @param[in] usPort             服务端端口
    * @param[in] bAsyncConnect      是否采用异步 Connnect
    * @return    TRUE 成功 FALSE 失败（可通过 GetLastError() 获取错误代码）
    */
    virtual BOOL Start(
        LPCWSTR pszRemoteAddress,
        USHORT usPort,
        BOOL bAsyncConnect = FALSE) = 0;

    /**
    * @brief     关闭通信组件
    * @return    TRUE 成功 FALSE 失败（可通过 GetLastError() 获取错误代码）
    */
    virtual BOOL Stop() = 0;

    /**
    * @brief     发送数据
    * @param[in] pBuffer    发送数据缓冲区
    * @param[in] iLength    发送数据长度
    * @return    TRUE 成功 FALSE 失败（可通过 GetLastError() 获取错误代码）
    */
    virtual BOOL Send(const BYTE* pBuffer, int iLength) = 0;

public:
    virtual BOOL HasStarted()                                           = 0;
    virtual emZLServiceState GetState()                                 = 0;
    virtual emZLClientError GetLastError()                              = 0;
    virtual ZLConnectId GetConnectionID()                               = 0;
    virtual BOOL GetLocalAddress(CStringW& strAddress, USHORT& usPort)  = 0;

    virtual void SetSocketBufferSize(DWORD dwSocketBufferSize)          = 0;
    virtual void SetKeepAliveTime(DWORD dwKeepAliveTime)                = 0;
    virtual void SetKeepAliveInterval(DWORD dwKeepAliveInterval)        = 0;
    virtual DWORD GetSocketBufferSize()                                 = 0;
    virtual DWORD GetKeepAliveTime()                                    = 0;
    virtual DWORD GetKeepAliveInterval()                                = 0;

public:
    virtual ~IZLClientSocket() {}
};
