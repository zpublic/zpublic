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
#include "z_lua_function_reg.hpp"

class z_lua_state
{
public:
    z_lua_state() : L(NULL), auto_close_(true)
    {}

    virtual ~z_lua_state()
    {
        if (auto_close())
        {
            close();
        }
    }

    ///> 是否在析构的时候自动关闭lua_State，默认为true
    bool auto_close() const
    {
        return auto_close_;
    }

    void auto_close(bool val)
    {
        auto_close_ = val;
    }

    ///> 创建和关闭lua_State，open等同于create，返回0为成功
    int open()
    {
        if (L)
        {
            return 1;
        }
        L = luaL_newstate();
        if (L == NULL)
        {
            return -1;
        }
        return 0;
    }

    int create()
    {
        return open();
    }

    int close()
    {
        if (L)
        {
            lua_close(L);
            L = NULL;
            return 0;
        }
        return 1;
    }

    ///> 加载脚本，执行全局的东东
    int dofile(const char *filename)
    {
        if (L)
        {
            int nRet = luaL_loadfile(L, filename);
            if (nRet != 0)
            {
                return -1;
            }
            nRet = lua_pcall(L, 0, LUA_MULTRET, 0);
            if (nRet != 0)
            {
                const char* pszErrorMsg = lua_tostring(L, -1);
                return -2;
            }
            return 0;
        }
        return -3;
    }

    int dostring(const char* buff)
    {
        return dobuffer(buff, strlen(buff));
    }

    int dobuffer(const char* buff, size_t sz)
    {
        if (L)
        {
            int nRet = luaL_loadbuffer(L, buff, sz, NULL);
            if (nRet != 0)
            {
                return -1;
            }
            nRet = lua_pcall(L, 0, LUA_MULTRET, 0);
            if (nRet != 0)
            {
                const char* pszErrorMsg = lua_tostring(L, -1);
                return -2;
            }
            return 0;
        }
        return -3;
    }

    ///> 注册lua脚本可以调用的标准库
    int open_all_libs()
    {
        if (L)
        {
            luaL_openlibs(L);
            return 0;
        }
        return -1;
    }

    int open_base()
    {
        if (L)
        {
            luaopen_base(L);
            return 0;
        }
        return -1;
    }

    int open_coroutine()
    {
        if (L)
        {
            luaopen_coroutine(L);
            return 0;
        }
        return -1;
    }

    int open_table()
    {
        if (L)
        {
            luaopen_table(L);
            return 0;
        }
        return -1;
    }

    int open_io()
    {
        if (L)
        {
            luaopen_io(L);
            return 0;
        }
        return -1;
    }

    int open_os()
    {
        if (L)
        {
            luaopen_os(L);
            return 0;
        }
        return -1;
    }

    int open_string()
    {
        if (L)
        {
            luaopen_string(L);
            return 0;
        }
        return -1;
    }

    int open_bit32()
    {
        if (L)
        {
            luaopen_bit32(L);
            return 0;
        }
        return -1;
    }

    int open_math()
    {
        if (L)
        {
            luaopen_math(L);
            return 0;
        }
        return -1;
    }

    int open_debug()
    {
        if (L)
        {
            luaopen_debug(L);
            return 0;
        }
        return -1;
    }

    int open_package()
    {
        if (L)
        {
            luaopen_package(L);
            return 0;
        }
        return -1;
    }

    ///> 注册函数给lua
    int reg_lib(const char *libname, const luaL_Reg* pfuncs, int nup = 0)
    {
        if (pfuncs && L)
        {
            if (!libname || *libname == 0)
            {
                libname = "z";
            }
            lua_getglobal(L, libname);
            if (lua_isnil(L, -1)) {
                lua_pop(L, 1);
                lua_newtable(L);
            }
            luaL_setfuncs(L, pfuncs, nup);
            lua_setglobal(L, libname);
            return 0;
        }
        return -1;
    }

    int reg_lib(z_lua_function_reg& funcs)
    {
        luaL_Reg* pfuncs = funcs.create();
        if (pfuncs)
        {
            reg_lib(funcs.libname().c_str(), pfuncs);
            funcs.close(pfuncs);
            return 0;
        }
        return -1;
    }

    int reg_function(const char *name, lua_CFunction func)
    {
        if (L)
        {
            if (name
                && *name != 0
                && func)
            {
                lua_register(L, name, func);
                return 0;
            }
            return -1;
        }
        return -2;
    }

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
