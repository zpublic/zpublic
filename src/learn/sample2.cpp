// sample2.cpp : C++ �� Lua ��ȣ���� ���� ������ �˾ƺ���.
//

#include "stdafx.h"
#include "sample_lua.h"

static int cpp_int = 100;

int main_l2()
{
    // Lua �� �ʱ�ȭ �Ѵ�.
    lua_State* L = lua_open();

    // Lua �⺻ �Լ����� �ε��Ѵ�.- print() ���
    luaopen_base(L);

    // LuaTinker �� �̿��ؼ� cpp_int �� Lua�� ����
    lua_tinker::set(L, "cpp_int", cpp_int);

    // sample1.lua ������ �ε�/�����Ѵ�.
    lua_tinker::dofile(L, "sample2.lua");

    // sample1.lua �� �Լ��� ȣ���Ѵ�.
    int lua_int = lua_tinker::get<int>(L, "lua_int");

    // lua_int �� ���
    printf("lua_int = %d\n", lua_int);

    // ���α׷� ����
    lua_close(L);

    return 0;
}
