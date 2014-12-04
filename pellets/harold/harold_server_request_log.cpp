#include "harold_header.h"
#include "harold_server_request_log.h"

int HaroldServerRequestLog::OnRequest(struct mg_connection *conn)
{
    assert(conn);
    HAROLD_PRINT("OnRequest:%s", conn->request_method);
    HAROLD_PRINT("OnRequest:%s", conn->uri);
    if (conn->query_string)
    {
        HAROLD_PRINT("OnRequest:%s", conn->query_string);
    }
    HAROLD_PRINT("OnRequest:%s", conn->remote_ip);
    HAROLD_PRINT("OnRequest:%s", conn->local_ip);
    return 0;
}
