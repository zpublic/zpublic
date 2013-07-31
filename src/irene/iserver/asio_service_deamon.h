#ifndef ASIO_SERVICE_DEAMON_H_
#define ASIO_SERVICE_DEAMON_H_

#include <io_service.h>
#include <network_common.h>
#include "service_deamon.h"

class TcpServer;
//class NetworkService;

class AsioServiceDeamon 
    : public ServiceDeamon<IOService>
{
public:
    AsioServiceDeamon();
    virtual ~AsioServiceDeamon();

public:
    virtual void start(const std::string& serviceName, 
        const uint32_t& threadNum = irene::net_params::smart_thread_nums());

    virtual void stop();

private:
    std::string _serviceName;
    TcpServer* _server;
    //NetworkService* _network_service;
};

#endif