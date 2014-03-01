// sample5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sample_lua.h"

void show_error(const char* error)
{
    printf("_ALERT -> %s\n", error);
}

int main_l5()
{
    // Lua �� �ʱ�ȭ �Ѵ�.
    lua_State* L = lua_open();

    // Lua �⺻ �Լ����� �ε��Ѵ�.- print() ���
    luaopen_base(L);


    // lua_State* �� �����ִ� ������ ������ �����ش�.
    printf("%s\n","-------------------------- current stack");
    lua_tinker::enum_stack(L);

    // ���ÿ� 1�� �о�ִ´�.
    lua_pushnumber(L, 1);

    // ���� ������ ������ �ٽ� ����Ѵ�.
    printf("%s\n","-------------------------- stack after push '1'");
    lua_tinker::enum_stack(L);


    // sample5.lua ������ �ε�/�����Ѵ�.
    lua_tinker::dofile(L, "sample5.lua");

    // test_error() �Լ��� ȣ���Ѵ�.
    // test_error() �� ������ test_error_3() ȣ���� �õ��ϴ� ������ �߻���Ų��.
    // �Լ� ȣ���� �߻��� ������ printf()�� ���ؼ� ��µȴ�.
    printf("%s\n","-------------------------- calling test_error()");
    lua_tinker::call<void>(L, "test_error");

    // test_error_3()�� �������� �ʴ� �Լ��̴�. ȣ�� ��ü�� �����Ѵ�.
    printf("%s\n","-------------------------- calling test_error_3()");
    lua_tinker::call<void>(L, "test_error_3");

    // printf() ��� ������ �����ϴ� ���� ��� ��ƾ�� ����� �� �ִ�.
    // �� ����ó�� �Լ���1���� ��� ���ڿ��� �߻��� ������ �����ϰ� �ȴ�.
    // C++ ���� ����� ��� void function(const char*) ���°� �����ϴ�.
    lua_tinker::def(L, "_ALERT", show_error);

    lua_tinker::call<void>(L, "_ALERT", "test !!!");

    // test_error() �Լ��� ȣ���Ѵ�.
    // �Լ� ȣ���� �߻��� ������ Lua�� ��ϵ� _ALERT()�� ���ؼ� ��µȴ�.
    printf("%s\n","-------------------------- calling test_error()");
    lua_tinker::call<void>(L, "test_error");


    // ���α׷� ����
    lua_close(L);

    return 0;
}

