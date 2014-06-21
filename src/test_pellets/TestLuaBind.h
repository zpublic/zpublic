#pragma once

#include "def.h"
#include "z_lua_bind/z_lua_bind.h"

static int f1(lua_State* L)
{
    return 0;
}
static int f2(lua_State* L)
{
    return 0;
}
static int f3(lua_State* L)
{
    return 0;
}

const std::string test_lua = 
    "               \
    hehe()          \
    z.zap_1()       \
    z.zap_2()       \
    b.zap_1()       \
    b.zap_2()       \
    g_pp = g_hehe   \
    function PI()   \
        return 3.14 \
    end             \
    function l_add(i,j)     \
        return i+j          \
    end                     \
    ";

class CTestLuaBind : public Suite
{
public:
    CTestLuaBind(void);
    ~CTestLuaBind(void);

    void testLuaBind()
    {
        z_lua_state l;
        TEST_ASSERT(0 == l.create())
        TEST_ASSERT(0 == l.open_all_libs())
        
        z_lua_function_reg reg;
        reg.prefix("zap_");
        reg.insert("1", f1);
        reg.insert("2", f2);
        TEST_ASSERT(0 == l.reg_lib(reg))
        reg.libname("b");
        TEST_ASSERT(0 == l.reg_lib(reg))

        l.set("g_hehe", 123);

        TEST_ASSERT(0 == l.reg_function("hehe", f3))
        TEST_ASSERT(0 == l.dostring(test_lua.c_str()))

        TEST_ASSERT(123 == l.get<int>("g_pp"))

        TEST_ASSERT(l.call<float>("PI") == 3.14)
        TEST_ASSERT(l.call<float>("l_add", 1.1, 2.2) == 3.3)
        TEST_ASSERT(l.call<int>("l_add", 3, 4) == 7)

        l.close();
    }
};
