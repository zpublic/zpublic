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
#include "stdafx.h"
#include "z_lua_state.h"


z_lua_state::z_lua_state() : L(NULL), auto_close_(true)
{

}

z_lua_state::~z_lua_state()
{
    if (auto_close())
    {
        close();
    }
}

bool z_lua_state::auto_close() const
{
    return auto_close_;
}

void z_lua_state::auto_close( bool val )
{
    auto_close_ = val;
}

int z_lua_state::open()
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

int z_lua_state::create()
{
    return open();
}

int z_lua_state::close()
{
    if (L)
    {
        lua_close(L);
        L = NULL;
        return 0;
    }
    return 1;
}

int z_lua_state::open_all_libs()
{
    if (L)
    {
        luaL_openlibs(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_base()
{
    if (L)
    {
        luaopen_base(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_coroutine()
{
    if (L)
    {
        luaopen_coroutine(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_table()
{
    if (L)
    {
        luaopen_table(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_io()
{
    if (L)
    {
        luaopen_io(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_os()
{
    if (L)
    {
        luaopen_os(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_string()
{
    if (L)
    {
        luaopen_string(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_bit32()
{
    if (L)
    {
        luaopen_bit32(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_math()
{
    if (L)
    {
        luaopen_math(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_debug()
{
    if (L)
    {
        luaopen_debug(L);
        return 0;
    }
    return -1;
}

int z_lua_state::open_package()
{
    if (L)
    {
        luaopen_package(L);
        return 0;
    }
    return -1;
}

int z_lua_state::reg_lib( const char *libname, const luaL_Reg* pfuncs, int nup /*= 0*/ )
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

int z_lua_state::reg_lib( z_lua_function_reg& funcs )
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

int z_lua_state::reg_function( const char *name, lua_CFunction func )
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

int z_lua_state::dofile( const char *filename )
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

int z_lua_state::dostring(const char* buff )
{
    return dobuffer(buff, strlen(buff));
}

int z_lua_state::dobuffer(const char* buff, size_t sz )
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
