#pragma once
#include "game_player.h"
#include "game_rome.h"

class game_session
    : public game_player
    , public std::enable_shared_from_this<game_session>
{
public:
    game_session(asio::io_service& io_service, game_room& room)
        : socket_(io_service)
        , room_(room)
    {
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        room_.join(shared_from_this());
        asio::async_read(socket_,
            asio::buffer(read_msg_.data(), game_message::header_length),
            std::bind(&game_session::handle_read_header,
            shared_from_this(),
            std::placeholders::_1));
    }

    void deliver(const game_message& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            asio::async_write(socket_,
                asio::buffer(write_msgs_.front().data(),
                write_msgs_.front().length()),
                std::bind(&game_session::handle_write, shared_from_this(),
                std::placeholders::_1));
        }
    }

    void handle_read_header(const asio::error_code& error)
    {
        if (!error && read_msg_.decode_header())
        {
            asio::async_read(socket_,
                asio::buffer(read_msg_.body(), read_msg_.body_length()),
                std::bind(&game_session::handle_read_body, shared_from_this(),
                std::placeholders::_1));
        }
        else
        {
            room_.leave(shared_from_this());
        }
    }

    void handle_read_body(const asio::error_code& error)
    {
        if (!error)
        {
            room_.deliver(read_msg_);
            asio::async_read(socket_,
                asio::buffer(read_msg_.data(), game_message::header_length),
                std::bind(&game_session::handle_read_header, shared_from_this(),
                std::placeholders::_1));
        }
        else
        {
            room_.leave(shared_from_this());
        }
    }

    void handle_write(const asio::error_code& error)
    {
        if (!error)
        {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
                asio::async_write(socket_,
                    asio::buffer(write_msgs_.front().data(),
                    write_msgs_.front().length()),
                    std::bind(&game_session::handle_write, shared_from_this(),
                    std::placeholders::_1));
            }
        }
        else
        {
            room_.leave(shared_from_this());
        }
    }

private:
    tcp::socket             socket_;
    game_room&              room_;
    game_message            read_msg_;
    game_message_queue      write_msgs_;
};

typedef std::shared_ptr<game_session> game_session_ptr;
