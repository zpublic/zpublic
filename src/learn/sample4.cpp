// sample4.cpp : table ���� ���� �˾ƺ���.
// table �� ���� ������ ���ؼ� stack �� table�� �����ؼ� ���� �ȴ�. 
// Lua ���� ���̺��� ���� �������� �����͸� �ö󰡰� ���� ��ü�� ���ο��� �����ȴ�.
// ���� �߸� ����� ��� ������ ���԰ų� Lua �������� ���� �����鿡 ������ ����ų ���� �ִ�.

#include "lua_tinker_ex.h"

int main_l4()
{
    // Lua �� �ʱ�ȭ �Ѵ�.
    lua_State* L = lua_open();

    // Lua �⺻ �Լ����� �ε��Ѵ�.- print() ���
    luaopen_base(L);

    {
        // Lua ���̺��� �����ϰ� ���ÿ� Ǫ���Ѵ�.
        lua_tinker::table haha(L, "haha");

        // haha.value = 1 ���� �ִ´�.
        haha.set("value", 1);

        // table ���� table�� ����� �ִ´�.
        haha.set("inside", lua_tinker::table(L));

        // haha.inside �� �����͸� �������� �����Ѵ�.
        lua_tinker::table inside = haha.get<lua_tinker::table>("inside");

        // inside.value = 2 ���� �ִ´�.
        inside.set("value", 2);

        // sample4.lua ������ �ε�/�����Ѵ�.
        lua_tinker::dofile(L, "sample4.lua");

        // Lua ���� ������ haha.test ���� �д´�.
        const char* test = haha.get<const char*>("test");
        printf("haha.test = %s\n", test);

        // ������ ������� �ʰ� Lua ���ÿ� �� ���̺��� �����Ѵ�.(��������)
        lua_tinker::table temp(L);

        // �� ���̺�.name �� ���� �ִ´�.
        temp.set("name", "local table !!");

        // table�� �� ���ڷ� ����Ͽ� print_table �� ȣ���Ѵ�.
        lua_tinker::call<void>(L, "print_table", temp);

        // �Լ��� �����ϴ� table�� �޴´�.
        lua_tinker::table ret = lua_tinker::call<lua_tinker::table>(L, "return_table", "give me a table !!");
        printf("ret.name =\t%s\n", ret.get<const char*>("name"));
    }

    // ���α׷� ����
    lua_close(L);

    return 0;
}

