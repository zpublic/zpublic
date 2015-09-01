#pragma once

///> 客户端操作结果代码
enum emZLClientError
{
    emZLClientErrorOk                   = 0,    // 成功
    emZLClientErrorIllegalState         = 1,    // 当前状态不允许操作
    emZLClientErrorInvalidParam         = 2,    // 非法参数
    emZLClientErrorSocketCreateFail     = 3,    // 创建 Client Socket 失败
    emZLClientErrorSocketPrepareFail    = 4,    // 设置 Client Socket 失败
    emZLClientErrorConnectServerFail    = 5,    // 连接服务器失败
    emZLClientErrorWorkerCreateFail     = 6,    // 创建工作线程失败
    emZLClientErrorNetworkError         = 7,    // 网络错误
    emZLClientErrorDataProcError        = 8,    // 数据处理错误
};

typedef std::list<zl::WinUtils::ZLBuffer*>      ZLBufferList;
