// sample2.cpp : C++ 와 Lua 상호간의 변수 접근을 알아본다.
//

#include "stdafx.h"
#include "sample_lua.h"

static int cpp_int = 100;

int main_l2()
{
    // Lua 를 초기화 한다.
    lua_State* L = lua_open();

    // Lua 기본 함수들을 로드한다.- print() 사용
    luaopen_base(L);

    // LuaTinker 를 이용해서 cpp_int 를 Lua에 전달
    lua_tinker::set(L, "cpp_int", cpp_int);

    // sample1.lua 파일을 로드/실행한다.
    lua_tinker::dofile(L, "sample2.lua");

    // sample1.lua 의 함수를 호출한다.
    int lua_int = lua_tinker::get<int>(L, "lua_int");

    // lua_int 값 출력
    printf("lua_int = %d\n", lua_int);

    // 프로그램 종료
    lua_close(L);

    return 0;
}
