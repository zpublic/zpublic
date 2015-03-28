#pragma once

///> Socket 监听器接口
class IZLSocketListener
{
public:
    /**
    * @brief     已发送数据通知
    * @param[in] connId         连接 ID
    * @param[in] pData          已发送数据缓冲区
    * @param[in] iLength        已发送数据长度
    * @return    HR_OK/HR_IGNORE 继续执行 , 不允许返回 HR_ERROR
    */
    virtual emZLSocketHandleResult OnSend(
        ZLConnectId connId,
        const BYTE* pData,
        int iLength) = 0;

    /**
    * @brief     已接收数据通知（PUSH 模型）
    * @param[in] connId         连接 ID
    * @param[in] pData          已接收数据缓冲区
    * @param[in] iLength        已接收数据长度
    * @return    HR_OK/HR_IGNORE 继续执行 , HR_ERROR引发 OnError() 事件时间并关闭连接
    */
    virtual emZLSocketHandleResult OnReceive(
        ZLConnectId connId,
        const BYTE* pData,
        int iLength) = 0;

    /**
    * @brief     数据到达通知（PULL 模型）
    * @param[in] connId         连接 ID
    * @param[in] iLength        已接收数据长度
    * @return    HR_OK/HR_IGNORE 继续执行 , HR_ERROR引发 OnError() 事件时间并关闭连接
    */
    virtual emZLSocketHandleResult OnReceive(ZLConnectId connId, int iLength) = 0;

    /**
    * @brief     已关闭连接通知
    * @param[in] connId         连接 ID
    * @return    void
    */
    virtual void OnClose(ZLConnectId connId) = 0;

    /**
    * @brief     通信错误通知
    * @param[in] connId         连接 ID
    * @param[in] emOperation    Socket 操作类型
    * @param[in] iErrorCode     错误代码
    * @return    void
    */
    virtual void OnError(
        ZLConnectId connId,
        emZLSocketOperation emOperation,
        int iErrorCode) = 0;

public:
    virtual ~IZLSocketListener() {}
};
