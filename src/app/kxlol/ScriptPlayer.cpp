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

int get_res_num(lua_State* L)
{
    int n = 0;
    const char* lpClassName = lua_tostring(L, 1);
	const char* lpName = lua_tostring(L, 2);
    if (lpName)
    {
        n = GAME.Player().RegBag().GetResNum(ZLA2W(lpClassName, CP_UTF8), ZLA2W(lpName, CP_UTF8));
    }
    lua_pushinteger(L, n);
    return 1;
}

int add_res_num(lua_State* L)
{
    const char* lpClassName = lua_tostring(L, 1);
	const char* lpName = lua_tostring(L, 2);
    int num = lua_tointeger(L, 3);
    if (lpName)
    {
        GAME.Player().RegBag().AddResNum(ZLA2W(lpClassName, CP_UTF8), ZLA2W(lpName, CP_UTF8), num);
    }
    return 0;
}

int sub_res_num(lua_State* L)
{
    bool b = false;
    const char* lpClassName = lua_tostring(L, 1);
	const char* lpName = lua_tostring(L, 2);
    int num = lua_tointeger(L, 3);
    if (lpName)
    {
        b = GAME.Player().RegBag().SubResNum(ZLA2W(lpClassName, CP_UTF8), ZLA2W(lpName, CP_UTF8), num);
    }
    lua_pushboolean(L, b);
    return 1;
}
