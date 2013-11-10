#include "room_manager.h"


RoomManager::RoomManager()
{

}
bool RoomManager::init()
{
    return true;
}

void RoomManager::destory()
{
    for (auto i = _rooms.begin(); i != _rooms.end(); ++i)
    {
        _roomPool.release(i->second);
    }
    _rooms.clear();
}

uint32 RoomManager::addRoom(const std::string& roomName, const std::string& password)
{
    if (_rooms.size() >= MAX_ROOM)
    {
        warning_log("Room count has reached the limit.");
        return ADD_ROOM_FAILED;
    }

    uint32 roomId = 0;
    if (_freeIds.empty())
    {
        roomId = _rooms.size() + 1;
    }
    else
    {
        roomId = _freeIds.back();
        _freeIds.pop_back();
    }
    _rooms[roomId] = _roomPool.acquire(roomId, roomName, password);
    return roomId;
}

void RoomManager::removeRoom(uint32 id)
{
    auto it = _rooms.find(id);
    if (it != _rooms.end())
    {
        _roomPool.release(it->second);
        _rooms.erase(it);
        _freeIds.push_back(id);
    }
    else
    {
        warning_log("Can't find the deleting room.");
    }
}

void RoomManager::getRooms(adap_map<uint32, Room*>& rooms)
{
    rooms = _rooms;
}

RoomManager::~RoomManager()
{

}
