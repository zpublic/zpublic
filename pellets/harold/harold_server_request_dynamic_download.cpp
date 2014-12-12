#include "harold_server_request_dynamic_download.h"

int HaroldServerRequestDynamicDownload::OnRequest(struct mg_connection *conn)
{
    if (conn->uri)
    {
        std::string sUri(conn->uri);
        std::string sFilePath;

        {
            z_mutex_guard guard(m_mutex);
            std::map<std::string, std::string>::iterator it = m_mapFileList.find(sUri);
            if (it == m_mapFileList.end())
            {
                goto Exit0;
            }
            sFilePath = it->second;
        }

        mg_send_file(conn, sFilePath.c_str(), NULL);
        return 3;
    }

Exit0:
    return 0;
}

bool HaroldServerRequestDynamicDownload::InsertDownload(const std::string& uri, const std::string& file)
{
    if (uri.size() == 0 || file.size() == 0)
    {
        return false;
    }

    {
        z_mutex_guard guard(m_mutex);
        m_mapFileList[uri] = file;
    }

    return true;
}

bool HaroldServerRequestDynamicDownload::RemoveDownload(const std::string& uri)
{
    if (uri.size() == 0)
    {
        return false;
    }

    {
        z_mutex_guard guard(m_mutex);
        std::map<std::string, std::string>::iterator it = m_mapFileList.find(uri);
        if (it != m_mapFileList.end())
        {
            m_mapFileList.erase(it);
        }
    }

    return true;
}
