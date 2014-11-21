#pragma once
#include "i_harold_server_observer.h"

///> http服务控制
class IHaroldServer
{
public:
    virtual bool StartServer()              = 0;
    virtual bool StopServer()               = 0;

    ///> 如果type为0，不会添加到任何类型
    virtual bool InsertObserver(
        IHaroldServerObserver* pObserver,
        unsigned int type)                  = 0;

    ///> 如果type为0，则尝试每种类型都删除
    virtual bool RomoveObserver(
        IHaroldServerObserver* pObserver,
        unsigned int type = 0)              = 0;
};
