#pragma once
#include "i_harold_server_request_observer.h"

class HaroldServerRequestLog : public IHaroldServerRequestObserver
{
public:
    virtual int OnRequest(struct mg_connection *conn);
};