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
    ";

class CTestLuaBind : public Suite
{
public:
    CTestLuaBind(void);
    ~CTestLuaBind(void);

    void testLuaBind()
    {
        z_lua_state l;
        l.create();
        l.open_all_libs();
        
        z_lua_function_reg reg;
        reg.prefix("zap_");
        reg.insert("1", f1);
        reg.insert("2", f2);
        l.reg_lib(reg);
        reg.libname("b");
        l.reg_lib(reg);

        l.reg_function("hehe", f3);
        l.dostring(test_lua.c_str());
        l.close();
    }
};