#pragma once

namespace zl
{
namespace LuaBind
{

    template<typename T>  
    void push(lua_State *L, T ret)
    {
        ///> 暂不支持复杂类型
        do { typedef int ai[1]; } while(0);
    }

    template<>	void push(lua_State *L, char ret);
    template<>	void push(lua_State *L, unsigned char ret);
    template<>	void push(lua_State *L, short ret);
    template<>	void push(lua_State *L, unsigned short ret);
    template<>	void push(lua_State *L, long ret);
    template<>	void push(lua_State *L, unsigned long ret);
    template<>	void push(lua_State *L, int ret);
    template<>	void push(lua_State *L, unsigned int ret);
    template<>	void push(lua_State *L, float ret);
    template<>	void push(lua_State *L, double ret);
    template<>	void push(lua_State *L, char* ret);
    template<>	void push(lua_State *L, const char* ret);
    template<>	void push(lua_State *L, bool ret);
    template<>	void push(lua_State *L, long long ret);
    template<>	void push(lua_State *L, unsigned long long ret);

}
}
