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

static float c_add(int a, int b, float c)
{
    return a+b+c;
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
    function cc(a,b,c)      \
        return c_add(a,b,c) \
    end                     \
    function test_ret(i)    \
        if i==1 then        \
            return \"aaa\"  \
        elseif i==2 then    \
            return {1,2,3}  \
        elseif i==3 then        \
            return {            \
                [\"a\"]=\"aa\", \
                [\"b\"]=\"bb\"} \
        end                 \
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

        TEST_ASSERT((int)l.call<float>("PI") == 3) 
        TEST_ASSERT((int)l.call<float>("l_add", 1.1, 2.2) == 3)
        TEST_ASSERT(l.call<int>("l_add", 3, 4) == 7)

        l.def("c_add", c_add);
        TEST_ASSERT((int)l.call<float>("cc", 1, 3, 2.2) == 6)

        std::string r1 = l.call<std::string>("test_ret", 1);
        TEST_ASSERT(r1 == "aaa")
        std::list<int> r2 = l.call<std::list<int> >("test_ret", 2);
        TEST_ASSERT(r2.size() == 3);
        std::vector<std::string> r3 = l.call<std::vector<std::string> >("test_ret", 3);
        TEST_ASSERT(r3.size() == 2);
        std::set<std::string> r4 = l.call<std::set<std::string> >("test_ret", 3);
        TEST_ASSERT(r4.size() == 2);
        std::map<std::string, std::string> r5 = l.call<std::map<std::string, std::string> >("test_ret", 3);
        TEST_ASSERT(r5.size() == 2);

        l.close();
    }
};
