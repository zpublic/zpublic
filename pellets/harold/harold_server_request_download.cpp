#include "harold_server_request_download.h"

int HaroldServerRequestDownload::OnRequest(struct mg_connection *conn)
{
    if (conn->uri)
    {
        std::string sUri(conn->uri);
        std::map<std::string, std::string>::iterator it = m_mapFileList.find(sUri);
        if (it != m_mapFileList.end())
        {
            mg_send_file(conn, it->second.c_str(), NULL);
            return 3;
        }
    }
    return 0;
}

bool HaroldServerRequestDownload::InsertDownload(const std::string& uri, const std::string& file)
{
    if (uri.size() == 0 || file.size() == 0)
    {
        return false;
    }
    m_mapFileList[uri] = file;
    return true;
}

bool HaroldServerRequestDownload::RemoveDownload(const std::string& uri)
{
    if (uri.size() == 0)
    {
        return false;
    }
    std::map<std::string, std::string>::iterator it = m_mapFileList.find(uri);
    if (it != m_mapFileList.end())
    {
        m_mapFileList.erase(it);
    }
    return true;
}
