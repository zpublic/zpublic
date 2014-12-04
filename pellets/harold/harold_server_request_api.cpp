#include "harold_server_request_api.h"

int HaroldServerRequestApi::OnRequest(struct mg_connection *conn)
{
    if (conn->uri)
    {
        std::string sUri(conn->uri);
        std::map<std::string, HaroldServerRequestApiFunc>::iterator it = m_mapApiList.find(sUri);
        if (it != m_mapApiList.end())
        {
            return it->second(conn);
        }
    }
    return 0;
}

bool HaroldServerRequestApi::InsertApi(const std::string& uri, HaroldServerRequestApiFunc func)
{
    if (uri.size() == 0 || !func)
    {
        return false;
    }
    m_mapApiList[uri] = func;
    return true;
}

bool HaroldServerRequestApi::RemoveApi(const std::string& uri)
{
    if (uri.size() == 0)
    {
        return false;
    }
    std::map<std::string, HaroldServerRequestApiFunc>::iterator it = m_mapApiList.find(uri);
    if (it != m_mapApiList.end())
    {
        m_mapApiList.erase(it);
    }
    return true;
}
