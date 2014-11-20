#pragma once
#include "i_harold_server_observer.h"

enum emHaroldServerStatus
{
    HaroldServerStatusStartBegin        = 1,
};

///> 状态观察者
///> 可以监控到服务器运转的各种内部状态
class IHaroldServerStatusObserver : public virtual IHaroldServerObserver
{
public:
    virtual unsigned int ObserverType() { return harold_observer_type_status; }
    virtual void OnStatus(emHaroldServerStatus status, void* pData) = 0;
};
