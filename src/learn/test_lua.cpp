#include "stdafx.h"
#include "test_lua.h"
#include "sample_lua.h"

void init_lua_test_file()
{
    string sFile = "\
function Calc(n)\n\
    return add_func(n,n+1);\n\
end\n";
    std::ofstream fo("./lua_test.lua");
    fo << sFile;
    fo.close();
}

lua_State* test_lua_init()
{
    lua_State* l = luaL_newstate();
    assert(l != NULL);
    int nRet = luaL_loadfile(l, "./lua_test.lua");
    assert(nRet == 0);
    nRet = lua_pcall(l, 0, 0, 0);
    assert(nRet == 0);
    lua_pop(l, 1);
    return l;
}

int lua_add_func(lua_State* l)
{
    int a = lua_tointeger(l, 1);
    int b = lua_tointeger(l, 2);
    lua_pushinteger(l, a + b);
    return 1;
}

void test_lua_export(lua_State* l)
{
    lua_pushcfunction(l, lua_add_func);
    lua_setglobal(l, "add_func");
}

void test_lua_run(lua_State* l)
{
    lua_getglobal(l,"Calc");
    lua_pushinteger(l, 100);
    int nRet = lua_pcall(l, 1, 1, 0);
    assert(nRet == 0);
    assert(lua_tointeger(l, -1) == 201);
    lua_pop(l, 1);
    lua_close(l);
}

void test_lua()
{
    init_lua_test_file();
    lua_State* l = test_lua_init();
    test_lua_export(l);
    test_lua_run(l);

    main_l1();
    main_l2();
    main_l3();
    main_l5();
    main_l6();
}
