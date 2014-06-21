#pragma once

namespace zl
{
namespace LuaBind
{

    template<typename T>  
    T read(lua_State *L, int index)
    {
        ///> 暂不支持复杂类型
        do { typedef int ai[1]; } while(0);
        T x;
        return x;
    }

    template<>	char*               read(lua_State *L, int index);
    template<>	const char*         read(lua_State *L, int index);
    template<>	char                read(lua_State *L, int index);
    template<>	unsigned char       read(lua_State *L, int index);
    template<>	short               read(lua_State *L, int index);
    template<>	unsigned short      read(lua_State *L, int index);
    template<>	long                read(lua_State *L, int index);
    template<>	unsigned long       read(lua_State *L, int index);
    template<>	int                 read(lua_State *L, int index);
    template<>	unsigned int        read(lua_State *L, int index);
    template<>	float               read(lua_State *L, int index);
    template<>	double              read(lua_State *L, int index);
    template<>	bool                read(lua_State *L, int index);
    template<>	void                read(lua_State *L, int index);
    template<>	long long           read(lua_State *L, int index);
    template<>	unsigned long long  read(lua_State *L, int index);

}}
