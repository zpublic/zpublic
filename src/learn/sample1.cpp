// sample1.cpp : C++ �� Lua ��ȣ���� �Լ� ������ �˾ƺ���.
//

#include "stdafx.h"
#include "sample_lua.h"

int cpp_func(int arg1, int arg2)
{
    return arg1 + arg2;
}

int main_l1()
{
    // Lua �� �ʱ�ȭ �Ѵ�.
    lua_State* L = lua_open();

    // Lua �⺻ �Լ����� �ε��Ѵ�.- print() ���
    luaopen_base(L);

    // LuaTinker �� �̿��ؼ� �Լ��� ����Ѵ�.
    lua_tinker::def(L, "cpp_func", cpp_func);

    // sample1.lua ������ �ε�/�����Ѵ�.
    lua_tinker::dofile(L, "sample1.lua");

    // sample1.lua �� �Լ��� ȣ���Ѵ�.
    int result = lua_tinker::call<int>(L, "lua_func", 3, 4);

    // lua_func(3,4) �� ����� ���
    printf("lua_func(3,4) = %d\n", result);

    // ���α׷� ����
    lua_close(L);

    return 0;
}
