#include "stdafx.h"
#include "harold_server_request_log.h"

int HaroldServerRequestLog::OnRequest(struct mg_connection *conn)
{
    assert(conn);
    std::cout << "OnRequest " << conn->request_method << std::endl;
    std::cout << "OnRequest " << conn->uri << std::endl;
    std::cout << "OnRequest " << conn->query_string << std::endl;
    std::cout << "OnRequest " << conn->remote_ip << std::endl;
    std::cout << "OnRequest " << conn->local_ip << std::endl;
    return 0;
}
