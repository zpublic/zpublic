#include "z_lua_bind\z_lua_bind.h"
#include <iostream>

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
    return a + b + c;
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
    
    l.set("g_hehe", 123);

    l.reg_function("hehe", f3);
    l.dostring(test_lua.c_str());

    int i = l.get<int>("g_pp");     //123
    std::cout << i << std::endl;
    i = l.call<float>("PI");        //3
    std::cout << i << std::endl;
    i = (int)l.call<float>("l_add", 1.1, 2.2);  //3
    std::cout << i << std::endl;
    i = l.call<int>("l_add", 3, 4);             //7
    std::cout << i << std::endl;

    l.def("c_add", c_add);
    i = (int)l.call<float>("cc", 1, 3, 2.2);    //6
    std::cout << i << std::endl;

    std::string r1 = l.call<std::string>("test_ret", 1);            ///aaa
    std::cout << r1 << std::endl;
    std::list<int> r2 = l.call<std::list<int> >("test_ret", 2);     //1 2 3
    std::cout << r2.size() << std::endl;
    std::vector<std::string> r3 = l.call<std::vector<std::string> >("test_ret", 3);     //aabb
    std::set<std::string> r4 = l.call<std::set<std::string> >("test_ret", 3);
    std::map<std::string, std::string> r5 = l.call<std::map<std::string, std::string> >("test_ret", 3);

    l.close();
}

int main()
{
    testLuaBind();
    getchar();
    return 0;
}
