#pragma once

namespace zl
{
namespace LuaBind
{

    template<typename F> 
    void def(lua_State* L, const char* name, F func)
    { 
        lua_pushlightuserdata(L, (void*)func);
        push_functor(L, func);
        lua_setglobal(L, name);
    }

}
}
