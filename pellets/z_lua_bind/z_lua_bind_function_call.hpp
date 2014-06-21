#pragma once

namespace zl
{
namespace LuaBind
{

    template<typename RVal>
    RVal call(lua_State* L, const char* name)
    {
        int nRet = lua_pcall(L, 0, 1, 0);
        lua_getglobal(L, name);
        if(lua_isfunction(L,-1))
        {
            lua_pcall(L, 0, 1, 0);
        }
        return pop<RVal>(L);
    }

    template<typename RVal, typename T1>
    RVal call(lua_State* L, const char* name, T1 arg)
    {
        lua_getglobal(L, name);
        if(lua_isfunction(L,-1))
        {
            push(L, arg);
            lua_pcall(L, 1, 1, 0);
        }
        return pop<RVal>(L);
    }

    template<typename RVal, typename T1, typename T2>
    RVal call(lua_State* L, const char* name, T1 arg1, T2 arg2)
    {
        lua_getglobal(L, name);
        if(lua_isfunction(L,-1))
        {
            push(L, arg1);
            push(L, arg2);
            lua_pcall(L, 2, 1, 0);
        }
        return pop<RVal>(L);
    }

    template<typename RVal, typename T1, typename T2, typename T3>
    RVal call(lua_State* L, const char* name, T1 arg1, T2 arg2, T3 arg3)
    {
        lua_getglobal(L, name);
        if(lua_isfunction(L,-1))
        {
            push(L, arg1);
            push(L, arg2);
            push(L, arg3);
            lua_pcall(L, 3, 1, 0);
        }
        return pop<RVal>(L);
    }

}
}
