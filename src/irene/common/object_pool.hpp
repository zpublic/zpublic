#ifndef OBJECT_POOL_H_
#define OBJECT_POOL_H_

#include "irene_common.h"
#include <boost/noncopyable.hpp>

template <typename _Ty>
class ObjectPool 
    : public boost::noncopyable
{
    static const uint32_t kDefaultSzie  = 0x00000080;   //128
    static const uint32_t kIncreaseSize = 0x00000400;   //1024

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
                obj->~_Ty();
                free(obj);
            }
        );
    }

public:
    _Ty* acquire()
    {
        try_allocate();
        _Ty* obj = get_free();
        try
        {
            new (obj) _Ty();
        }
        catch (...)
        {
            free(obj); throw;
        }

        return obj;
    }

    template <typename ArgT0>
    _Ty* acquire(const ArgT0& arg0)
    {
        try_allocate();
        _Ty* obj = get_free();
        try
        {
            new (obj) _Ty(arg0);
        }
        catch (...)
        {
            free(obj); throw;
        }

        return obj;
    }

    template <typename ArgT0, typename ArgT1>
    _Ty* acquire(const ArgT0& arg0, const ArgT1& arg1)
    {
        try_allocate();
        _Ty* obj = get_free();
        try
        {
            new (obj) _Ty(arg0, arg1);
        }
        catch (...)
        {
            free(obj); throw;
        }

        return obj;
    }

    template <typename ArgT0, typename ArgT1, typename ArgT2>
    _Ty* acquire(const ArgT0& arg0, const ArgT1& arg1, const ArgT2& arg2)
    {
        try_allocate();
        _Ty* obj = get_free();
        try
        {
            new (obj) _Ty(arg0, arg1, arg2);
        }
        catch (...)
        {
            free(obj); throw;
        }
    
        return obj;
    }

    void release(_Ty* obj)
    {
        _free_objects.push(obj);
    }

private:
    _Ty* get_free()
    {
        _Ty* obj = _free_objects.top();
        _free_objects.pop();

        std::cout << "free objects count = " << _free_objects.size() << std::endl;

        return obj;
    }

    void try_allocate()
    {
        if (_free_objects.empty())
            allocate(_increase_size);
    }

    void allocate(size_t size)
    {
        _Ty* obj = static_cast<_Ty*>(malloc(size * sizeof(_Ty)));
        _all_objects.push_back(obj);

        for (size_t i = 0; i < size; ++i)
            _free_objects.push(&obj[i]);

        std::cout << "allocated free objects = " << size << ", total bytes = " << size *  sizeof(obj[0]) << std::endl;
    }

private:
    uint32_t _chunk_size;
    uint32_t _increase_size;
    std::stack<_Ty*> _free_objects;
    std::vector<_Ty*> _all_objects;
};


#endif //OBJECT_POOL_H_