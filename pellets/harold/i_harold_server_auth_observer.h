#pragma once
#include "i_harold_server_observer.h"

///> 验证观察者
///> 默认都返回MG_TRUE
///> OnAuth返回>0 则直接返回MG_TRUE，不再走其它AuthObserver
///> OnAuth返回<0 则直接返回MG_FALSE，不再走其它AuthObserver
///> OnAuth返回=0 则继续走其它AuthObserver
class IHaroldServerAuthObserver : public virtual IHaroldServerObserver
{
public:
    virtual unsigned int ObserverType() { return harold_observer_type_auth; }
    virtual int OnAuth(struct mg_connection *conn) = 0;
};
