#pragma once
#include "i_harold_server_request_observer.h"
#include "..\z_platform\z_platform_mutex.h"
#include <string>
#include <map>

class HaroldServerRequestDynamicDownload : public IHaroldServerRequestObserver
{
public:
    virtual int OnRequest(struct mg_connection *conn);

    bool InsertDownload(const std::string& uri, const std::string& file);
    bool RemoveDownload(const std::string& uri);

protected:
    zl_mutex                                 m_mutex;
    std::map<std::string, std::string>      m_mapFileList;
};
