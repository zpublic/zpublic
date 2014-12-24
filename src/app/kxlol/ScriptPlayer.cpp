#include "stdafx.h"
#include "ScriptPlayer.h"


int get_name(lua_State* L)
{
    return 0;
}

int set_name(lua_State* L)
{
    return 0;
}

int get_shenli(lua_State* L)
{
    lua_pushinteger(L, (lua_Integer)GAME.Player().BasicInfo().GetShenli());
    return 1;
}

int set_shenli(lua_State* L)
{
    ULONGLONG ull = lua_tointeger(L, 1);
    GAME.Player().BasicInfo().SetShenli(ull);
    return 0;
}

int add_shenli(lua_State* L)
{
    ULONGLONG ull = lua_tointeger(L, 1);
    GAME.Player().BasicInfo().AddShenli(ull);
    return 0;
}

int sub_shenli(lua_State* L)
{
    ULONGLONG ull = lua_tointeger(L, 1);
    GAME.Player().BasicInfo().SubShenli(ull);
    return 0;
}
