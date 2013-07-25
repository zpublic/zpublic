#ifndef ASIO_SERVICE_DEAMON_H_
#define ASIO_SERVICE_DEAMON_H_

#include "common_def.h"
#include "service_deamon.h"
#include "io_service.h"

class TcpServer;
class NetworkService;

class AsioServiceDeamon 
    : public ServiceDeamon<IOService>
{
public:
    AsioServiceDeamon();
    virtual ~AsioServiceDeamon();

public:
    virtual void start(const std::string& serviceName);
    virtual void stop();

private:
    std::string _serviceName;
    TcpServer* _server;
    NetworkService* _network_service;
};

#endif