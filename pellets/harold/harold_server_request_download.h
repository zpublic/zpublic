#pragma once
#include "i_harold_server_request_observer.h"
#include "..\thread_sync\thread_sync.h"
#include <string>
#include <map>

class HaroldServerRequestDownload : public IHaroldServerRequestObserver
{
public:
    virtual int OnRequest(struct mg_connection *conn);

    bool InsertDownload(const std::string& uri, const std::string& file);
    bool RemoveDownload(const std::string& uri);

protected:
    z_mutex                                 m_mutex;
    std::map<std::string, std::string>      m_mapFileList;
};
