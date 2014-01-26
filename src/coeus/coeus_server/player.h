#ifndef __PLAYER_H__
#define __PLAYER_H__

class GameSession;

class Player
{
public:
    Player(uint64 playerId, GameSession* session);
    virtual ~Player();

private:
    PlayerDB* _playerDB;
};

#endif