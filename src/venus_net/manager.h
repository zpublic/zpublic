#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "singleton.h"
#include <hash_map>

namespace Venus
{
template <typename ID_TYPE, typename OBJ_TYPE>
class ObjectManager
    : public Venus::Singleton<ObjectManager<ID_TYPE, OBJ_TYPE>>
{
protected:
    virtual bool init() { return true; };
    virtual void destroy() {};

public:
    bool add(const ID_TYPE& id, OBJ_TYPE* obj)
    {
        if (get(id) != nullptr)
        {
            return false;
        }

        _mutex.lock();
        _objList.insert(std::make_pair(id, obj));
        _mutex.unlock();

        return true;
    }

    void remove(const ID_TYPE& id)
    {
        auto iter = _objList.find(id);
        if (iter != _objList.end())
        {
            _mutex.lock();
            _objList.erase(iter);
            _mutex.unlock();
        }
    }

    OBJ_TYPE* get(const ID_TYPE& id)
    {
        auto iter = _objList.find(id);
        if (iter != _objList.end())
        {
            return iter->second;
        }

        return nullptr;
    }

    void clear()
    {
        _objList.clear();
    }

private:
    std::mutex _mutex;
    std::hash_map<ID_TYPE, OBJ_TYPE*> _objList;
};

}

#endif