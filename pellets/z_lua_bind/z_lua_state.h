/*************************************************************************
 *                                                                       *
 *  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
 *  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
 *        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
 *       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
 *     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
 *    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
 *   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
 * !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
 *                                                                       *
 *                               ZPublic                                 *
 *                  Developer: zapline(278998871@qq.com)                 *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#pragma once
#include <string.h>
#include "z_lua_function_reg.h"

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

    ///> 加载脚本，执行全局的东东
    int dofile(const char *filename);
    int dostring(const char* buff);
    int dobuffer(const char* buff, size_t sz);

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

    ///> 全局变量
    template<typename T>
    void set(const char* name, T object)
    {
        zl::LuaBind::push(L, object);
        lua_setglobal(L, name);
    }
    template<typename T>
    T get(const char* name)
    {
        lua_getglobal(L, name);
        return zl::LuaBind::pop<T>(L);
    }

    ///> 注册全局函数给lua
    template<typename F> 
    void def(const char* name, F func)
    { 
        lua_pushlightuserdata(L, (void*)func);
        zl::LuaBind::push_functor(L, func);
        lua_setglobal(L, name);
    }

    ///> 全局lua函数调用
    template<typename RVal>
    RVal call(const char* name)
    {
        int nRet = lua_pcall(L, 0, 1, 0);
        lua_getglobal(L, name);
        if(lua_isfunction(L,-1))
        {
            lua_pcall(L, 0, 1, 0);
        }
        return zl::LuaBind::pop<RVal>(L);
    }
    template<typename RVal, typename T1>
    RVal call(const char* name, T1 arg)
    {
        lua_getglobal(L, name);
        if(lua_isfunction(L,-1))
        {
            zl::LuaBind::push(L, arg);
            lua_pcall(L, 1, 1, 0);
        }
        return zl::LuaBind::pop<RVal>(L);
    }
    template<typename RVal, typename T1, typename T2>
    RVal call(const char* name, T1 arg1, T2 arg2)
    {
        lua_getglobal(L, name);
        if(lua_isfunction(L,-1))
        {
            zl::LuaBind::push(L, arg1);
            zl::LuaBind::push(L, arg2);
            lua_pcall(L, 2, 1, 0);
        }
        return zl::LuaBind::pop<RVal>(L);
    }
    template<typename RVal, typename T1, typename T2, typename T3>
    RVal call(const char* name, T1 arg1, T2 arg2, T3 arg3)
    {
        lua_getglobal(L, name);
        if(lua_isfunction(L,-1))
        {
            zl::LuaBind::push(L, arg1);
            zl::LuaBind::push(L, arg2);
            zl::LuaBind::push(L, arg3);
            lua_pcall(L, 3, 1, 0);
        }
        return zl::LuaBind::pop<RVal>(L);
    }

protected:
    lua_State* L;
    bool auto_close_;
};
