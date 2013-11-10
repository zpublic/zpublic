#ifndef __ROOM_H__
#define __ROOM_H__

#include <common.h>
class Room
{
public:
    Room(uint32 id, const std::string& roomName, const std::string& password) 
    {
        _id = id;
        _roomName = roomName;
        _password = password;
    }
    uint32 getId() { return _id; }
    std::string getRoomName() { return _roomName; }
    std::string getPassword() { return _password; }
    std::string getOwner() { return _owner; }
    void getMembers(std::vector<std::string>& members) { members = _members; }
    uint32 getPlayerCount() { return _members.size(); }
protected:
private:
    std::string _roomName;
    std::string _password;
    std::string _owner;
    std::vector<std::string> _members;
    uint32 _id;
};
#endif