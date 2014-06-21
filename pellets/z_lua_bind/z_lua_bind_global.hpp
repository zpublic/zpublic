#pragma once

// global variable
template<typename T>
void set(lua_State* L, const char* name, T object)
{
    push(L, object);
    lua_setglobal(L, name);
}

template<typename T>
T get(lua_State* L, const char* name)
{
    lua_getglobal(L, name);
    return pop<T>(L);
}
