#include "stdafx.h"
#include "z_lua_bind_internal_read.h"

namespace zl
{
namespace LuaBind
{

    template<>
    char* read(lua_State *L, int index)
    {
        return (char*)lua_tostring(L, index);
    }

    template<>
    const char* read(lua_State *L, int index)
    {
        return (const char*)lua_tostring(L, index);		
    }

    template<>
    char read(lua_State *L, int index)
    {
        return (char)lua_tonumber(L, index);
    }

    template<>
    unsigned char read(lua_State *L, int index)
    {
        return (unsigned char)lua_tonumber(L, index);
    }

    template<>
    short read(lua_State *L, int index)
    {
        return (short)lua_tonumber(L, index);
    }

    template<>
    unsigned short read(lua_State *L, int index)
    {
        return (unsigned short)lua_tonumber(L, index);
    }

    template<>
    long read(lua_State *L, int index)
    {
        return (long)lua_tonumber(L, index);
    }

    template<>
    unsigned long read(lua_State *L, int index)
    {
        return (unsigned long)lua_tonumber(L, index);
    }

    template<>
    int read(lua_State *L, int index)
    {
        return (int)lua_tonumber(L, index);
    }

    template<>
    unsigned int read(lua_State *L, int index)
    {
        return (unsigned int)lua_tonumber(L, index);
    }

    template<>
    float read(lua_State *L, int index)
    {
        return (float)lua_tonumber(L, index);
    }

    template<>
    double read(lua_State *L, int index)
    {
        return (double)lua_tonumber(L, index);
    }

    template<>
    bool read(lua_State *L, int index)
    {
        if(lua_isboolean(L, index))
            return lua_toboolean(L, index) != 0;
        else
            return lua_tonumber(L, index) != 0;
    }

    template<>
    void read(lua_State *L, int index)
    {
        return;
    }

    template<>
    long long read(lua_State *L, int index)
    {
        if(lua_isnumber(L,index))
            return (long long)lua_tonumber(L, index);
        else
            return *(long long*)lua_touserdata(L, index);
    }

    template<>
    unsigned long long read(lua_State *L, int index)
    {
        if(lua_isnumber(L,index))
            return (unsigned long long)lua_tonumber(L, index);
        else
            return *(unsigned long long*)lua_touserdata(L, index);
    }

}
}
