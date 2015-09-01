#pragma once
#include "i_harold_server_auth_observer.h"

class HaroldServerAuthLog : public IHaroldServerAuthObserver
{
public:
    virtual int OnAuth(struct mg_connection *conn);
};
