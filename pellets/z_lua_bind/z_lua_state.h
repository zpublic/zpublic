#pragma once
#include <string.h>

class z_lua_state
{
public:
    z_lua_state();
    virtual ~z_lua_state();

    ///> 是否在析构的时候自动关闭lua_State，默认为true
    bool auto_close() const;
    void auto_close(bool val);

    ///> 创建和关闭lua_State，open等同于create，返回0为成功
    int open();
    int create();
    int close();

    ///> 注册lua脚本可以调用的标准库
    int open_all_libs();
    int open_base();
    int open_coroutine();
    int open_table();
    int open_io();
    int open_os();
    int open_string();
    int open_bit32();
    int open_math();
    int open_debug();
    int open_package();

    ///> 注册函数给lua
    int reg_lib(const char *libname, const luaL_Reg* pfuncs, int nup = 0);
    int reg_lib(z_lua_function_reg& funcs);
    int reg_function(const char *name, lua_CFunction func);

    ///> 加载脚本，执行全局的东东
    int dofile(const char *filename);
    int dostring(const char* buff);
    int dobuffer(const char* buff, size_t sz);

protected:
    lua_State* L;
    bool auto_close_;
};
