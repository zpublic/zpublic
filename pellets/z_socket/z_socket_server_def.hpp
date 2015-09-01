#pragma once

///> 服务端操作结果代码
enum emZLServerError
{
    emZLServerErrorOk                   = 0,    // 成功
    emZLServerErrorIllegalState         = 1,    // 当前状态不允许操作
    emZLServerErrorInvalidParam         = 2,    // 非法参数
    emZLServerErrorSocketCreateFail     = 3,    // 创建 Socket 失败
    emZLServerErrorSocketBindFail       = 4,    // 绑定监听地址失败
    emZLServerErrorSocketPrepareFail    = 5,    // 设置监听 Socket 失败
    emZLServerErrorSocketListenFail     = 6,    // 启动监听失败
    emZLServerErrorCpCreateFail         = 7,    // 创建完成端口失败
    emZLServerErrorWorkerCreateFail     = 8,    // 创建工作线程失败
    emZLServerErrorSocketAttachCpFail   = 9,    // 监听 Socket 绑定到完成端口失败
    emZLServerErrorAcceptCreateFail     = 10,   // 创建监听线程失败
    emZLServerErrorGetFuncPtrFail       = 11,   // WSAIoctl取函数地址失败
};

///> 数据缓冲区
typedef struct _TBufferObj
{
    OVERLAPPED          ov;
    WSABUF              buf;
    emZLSocketOperation oper;
    SOCKET              client;
}TBufferObj;

///> socket缓冲区
typedef struct _TSocketObj
{
    SOCKET              socket;
    sockaddr_in         clientAddr;
    ZLConnectId         connId;
    z_mutex*            pmutex;
}TSocketObj;

typedef std::list<TBufferObj*>              TBufferObjList;
typedef std::list<TSocketObj*>              TSocketObjList;
typedef std::map<ZLConnectId, TSocketObj*>  TSocketObjMap;
