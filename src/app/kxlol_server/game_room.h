#pragma once
#include "game_player.h"
#include <set>
#include <algorithm>
#include <functional>

class game_room
{
public:
    void join(game_player_ptr player)
    {
        players_.insert(player);
    }

    void leave(game_player_ptr player)
    {
        players_.erase(player);
    }

    void deliver(const game_message& msg)
    {
        std::for_each(
            players_.begin(),
            players_.end(),
            std::bind(&game_player::deliver, std::placeholders::_1, std::ref(msg)));
    }

private:
    std::set<game_player_ptr> players_;
};
