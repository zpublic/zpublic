/********************************************************************
* @file      : hash.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/09 9:47
* @brief     : ¹þÏ£
* 
* 
*********************************************************************/

#ifndef _HASH_HPP_
#define _HASH_HPP_

namespace zl
{

class CHash
{
public:
    template<typename KeyT>
    static DWORD HashKey(KeyT Key)
    {
        ldiv_t hash_val = ldiv((long)Key, 127773);
        hash_val.rem = 16807 * hash_val.rem - 2836 * hash_val.quot;
        if (hash_val.rem < 0) hash_val.rem += 2147483647;
        return ((UINT)hash_val.rem);
    }

    template<>
    static DWORD HashKey<LPCSTR>(LPCSTR Key)
    {
        DWORD hash_val = 2166136261U;
        DWORD fst = 0;
        DWORD lst = (DWORD)strlen(Key);
        DWORD stride = 1 + lst / 10;
        for(; fst < lst; fst += stride)
            hash_val = 16777619U * hash_val ^ (DWORD)Key[fst];
        return (hash_val);
    }

    template<>
    static DWORD HashKey<LPCWSTR>(LPCWSTR Key)
    {
        DWORD hash_val = 2166136261U;
        DWORD fst = 0;
        DWORD lst = (DWORD)wcslen(Key);
        DWORD stride = 1 + lst / 10;
        for(; fst < lst; fst += stride)
            hash_val = 16777619U * hash_val ^ (DWORD)Key[fst];
        return (hash_val);
    }

    template<>
    static DWORD HashKey<__int64>(__int64 key)
    {
        return (HashKey<DWORD>((DWORD)(key & 0xffffffffUL)) ^ HashKey<DWORD>((DWORD)(key >> 32)));
    }
};

}

#endif