// kxlol_server_tester.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "game_client.h"
#include <functional>
#include <thread>


int _tmain(int argc, _TCHAR* argv[])
{
    asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("127.0.0.1", "48360");
    tcp::resolver::iterator iterator = resolver.resolve(query);

    game_client c(io_service, iterator);

    //std::thread t(
    //    std::bind(
    //    static_cast<size_t(asio::io_service::*)()>(&asio::io_service::run),
    //    &io_service));
    std::thread t([&]
    {
        io_service.run();
    });

    char line[game_message::max_body_length + 1];
    while (std::cin.getline(line, game_message::max_body_length + 1))
    {
        game_message msg;
        msg.body_length(strlen(line));
        memcpy(msg.body(), line, msg.body_length());
        msg.encode_header();
        c.write(msg);
    }

    c.close();
    t.join();

	return 0;
}

