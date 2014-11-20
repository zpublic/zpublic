#include "stdafx.h"
#include "harold_server_auth_log.h"

int HaroldServerAuthLog::OnAuth(struct mg_connection *conn)
{
    assert(conn);
    std::cout << "OnAuth " << conn->request_method << std::endl;
    std::cout << "OnAuth " << conn->uri << std::endl;
    if (conn->query_string)
    {
        std::cout << "OnAuth " << conn->query_string << std::endl;
    }
    std::cout << "OnAuth " << conn->remote_ip << std::endl;
    std::cout << "OnAuth " << conn->local_ip << std::endl;
    return 0;
}

