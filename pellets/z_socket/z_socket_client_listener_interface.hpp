#pragma once

///> 客户端 Socket 监听器接口
class IZLClientSocketListener : public IZLSocketListener
{
public:
    /**
    * @brief     准备连接通知（进行Socket 选项设置等额外工作）
    * @param[in] connId         连接 ID
    * @param[in] socket         客户端 Socket
    * @return    HR_OK / HR_IGNORE 继续执行，HR_ERROR 终止启动
    */
    virtual emZLSocketHandleResult OnPrepareConnect(ZLConnectId connId, SOCKET socket) = 0;

    /**
    * @brief     连接完成通知（进行Socket 选项设置等额外工作）
    * @param[in] connId         连接 ID
    * @return    HR_OK / HR_IGNORE 继续执行，HR_ERROR 同步连接终止启动 异步连接关闭连接
    */
    virtual emZLSocketHandleResult OnConnect(ZLConnectId connId) = 0;
};
