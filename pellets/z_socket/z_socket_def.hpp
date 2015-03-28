#pragma once

///> 连接ID的类型
typedef ULONG ZLConnectId;

///> 事件通知处理结果（影响通信组件的后续行为）
enum emZLSocketHandleResult
{
    emZLSocketHandleResultOk        = 0,        ///> 成功
    emZLSocketHandleResultIgnore    = 1,        ///> 忽略
    emZLSocketHandleResultError     = 2,        ///> 错误
};

///> Socket 操作类型（用于OnErrror标识导致错误的操作类型）
enum emZLSocketOperation
{
    emZLSocketOperationUnknown  = 0,
    emZLSocketOperationAccept   = 1,
    emZLSocketOperationConnect  = 2,
    emZLSocketOperationSend     = 3,
    emZLSocketOperationReceive  = 4,
};

///> 通信组件服务状态
enum emZLServiceState
{
    emZLServiceStateStarting    = 0,    // 正在启动
    emZLServiceStateStarted     = 1,    // 已经启动
    emZLServiceStateStoping     = 2,    // 正在停止
    emZLServiceStateStoped      = 3,    // 已经启动
};
