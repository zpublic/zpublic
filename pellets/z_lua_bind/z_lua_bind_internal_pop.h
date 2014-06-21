#pragma once

namespace zl
{
namespace LuaBind
{

    template<typename T>  
    T pop(lua_State *L)
    {
        T t = read<T>(L, -1);
        lua_pop(L, 1);
        return t;
    }

    template<> void pop(lua_State *L);

}}
