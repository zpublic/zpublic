#pragma once
#include "i_harold_server_request_observer.h"
#include <string>
#include <map>

typedef int(*HaroldServerRequestApiFunc)(struct mg_connection *);

class HaroldServerRequestApi : public IHaroldServerRequestObserver
{
public:
    virtual int OnRequest(struct mg_connection *conn);

    bool InsertApi(const std::string& uri, HaroldServerRequestApiFunc func);
    bool RemoveApi(const std::string& uri);

protected:
    std::map<std::string, HaroldServerRequestApiFunc>      m_mapApiList;
};
