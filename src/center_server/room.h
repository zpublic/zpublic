#ifndef __ROOM_H__
#define __ROOM_H__

#include <common.h>
class Room
{
public:
    Room(uint32 id, const std::string& roomName, const std::string& password, uint64 ownerID) 
    {
        _id = id;
        _roomName = roomName;
        _password = password;
        _ownerGuid = ownerID;
    }
    uint32 getId() { return _id; }
    std::string getRoomName() { return _roomName; }
    std::string getPassword() { return _password; }
    uint64 getOwner() { return _ownerGuid; }
    void getMembers(std::list<uint64>& members) { members = _membersGuid; }
    void addMember(uint64 playerGuid) { _membersGuid.push_back(playerGuid); }
    void removeMember(uint64 playerGuid) { _membersGuid.remove(playerGuid); }
    
    uint32 getPlayersCount() { return _membersGuid.size() + 1; }
protected:

    
private:
    std::string _roomName;
    std::string _password;
    uint64 _ownerGuid;
    std::list<uint64> _membersGuid;
    uint32 _id;
};
#endif