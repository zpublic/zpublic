#pragma once
#include "i_harold_server_observer.h"

///> 请求观察者
///> 默认都返回MG_TRUE
///> OnRequest返回1 则直接返回MG_TRUE，不再走其它RequestObserver
///> OnRequest返回2 则直接返回MG_FALSE，不再走其它RequestObserver
///> OnRequest返回3 则直接返回MG_MORE，不再走其它RequestObserver
///> OnRequest返回0 则继续走其它RequestObserver(其它值)
class IHaroldServerRequestObserver : public virtual IHaroldServerObserver
{
public:
    virtual unsigned int ObserverType() { return harold_observer_type_request; }
    virtual int OnRequest(struct mg_connection *conn) = 0;
};
