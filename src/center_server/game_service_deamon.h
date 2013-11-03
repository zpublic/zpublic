#ifndef __GAME_SERVICE_DEAMON_H__
#define __GAME_SERVICE_DEAMON_H__

#include <io_service.h>
#include <network_common.h>
#include <network_service_deamon.h>

class TcpServer;
class GameServiceDeamon 
    : public Venus::NetworkServiceDeamon
{
public:
    GameServiceDeamon(const std::string& service_name, IOService& io_service);
    ~GameServiceDeamon();

public:
    void start(const uint16& port, const uint32& threadNum = Venus::smart_thread_nums());
    void stop();

private:
    TcpServer* _server;
};

#endif