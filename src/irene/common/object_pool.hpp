#ifndef OBJECT_POOL_H_
#define OBJECT_POOL_H_

#include "irene_common.h"
#include <boost/noncopyable.hpp>

template <typename _Ty>
class ObjectPool 
    : public boost::noncopyable
{
    static const uint32_t kDefaultSzie  = 0x00000100;
    static const uint32_t kIncreaseSize = 0x00000100;

public:
    explicit ObjectPool(uint32_t chunk_size = kDefaultSzie, uint32_t increase_size = kIncreaseSize)
        : _chunk_size(chunk_size), _increase_size(increase_size)
    {
        if (_chunk_size <= 0)
            throw std::invalid_argument("invalid chunk size");

        allocate(_chunk_size);
    }

    ~ObjectPool()
    {
        std::for_each(_all_objects.begin(), _all_objects.end(), [](_Ty* obj)
            {
                delete [] obj;
            }
        );
    }

public:
    _Ty* acquire()
    {
        if (_free_objects.empty())
            allocate(_increase_size);

        _Ty* obj = _free_objects.front();
        _free_objects.pop();

        return obj;
    }

    void release(_Ty* obj)
    {
        _free_objects.push(obj);
    }

private:
    void allocate(uint32_t size)
    {
        _Ty* obj = new _Ty[size];
        _all_objects.push_back(obj);
        for (size_t i = 0; i < size; ++i)
        {
            _free_objects.push(&obj[i]);
        }
     }

private:
    uint32_t _chunk_size;
    uint32_t _increase_size;
    std::queue<_Ty*> _free_objects;
    std::vector<_Ty*> _all_objects;
};


#endif //OBJECT_POOL_H_