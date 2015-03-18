#pragma once
#include "game_room.h"
#include "game_session.h"

class game_server
{
public:
    game_server(asio::io_service& io_service,
        const tcp::endpoint& endpoint)
        : io_service_(io_service),
        acceptor_(io_service, endpoint)
    {
        start_accept();
    }

    void start_accept()
    {
        game_session_ptr new_session(new game_session(io_service_, room_));
        acceptor_.async_accept(new_session->socket(),
            std::bind(&game_server::handle_accept, this, new_session,
            std::placeholders::_1));
    }

    void handle_accept(game_session_ptr session,
        const asio::error_code& error)
    {
        if (!error)
        {
            session->start();
        }

        start_accept();
    }

private:
    asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    game_room room_;
};

typedef std::shared_ptr<game_server> game_server_ptr;
