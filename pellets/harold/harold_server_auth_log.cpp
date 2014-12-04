#include "harold_header.h"
#include "harold_server_auth_log.h"

int HaroldServerAuthLog::OnAuth(struct mg_connection *conn)
{
    assert(conn);
    HAROLD_PRINT("OnAuth:%s", conn->request_method);
    HAROLD_PRINT("OnAuth:%s", conn->uri);
    if (conn->query_string)
    {
        HAROLD_PRINT("OnAuth:%s", conn->query_string);
    }
    HAROLD_PRINT("OnAuth:%s", conn->remote_ip);
    HAROLD_PRINT("OnAuth:%s", conn->local_ip);
    return 0;
}
