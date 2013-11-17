#ifndef ROOM_POOL_
#define ROOM_POOL_
#include <singleton.h>
#include <object_pool.hpp>
#include "room.h"

class RoomPool
    : public Venus::ObjectPool<Room>, public Venus::Singleton<RoomPool>
{
public:
    Room* acquire(uint32 id, const std::string& roomName, const std::string& password, uint64 ownerGuid)
    {
        return ObjectPool::acquire(id, roomName, password, ownerGuid);
    }
protected:
private:
};


#endif // !ROOM_POOL_
