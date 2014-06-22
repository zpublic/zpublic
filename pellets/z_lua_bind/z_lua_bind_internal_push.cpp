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
#include "z_lua_bind_internal_push.h"

namespace zl
{
namespace LuaBind
{

template<>
void push(lua_State *L, char ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, unsigned char ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, short ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, unsigned short ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, long ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, unsigned long ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, int ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, unsigned int ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, float ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, double ret)
{
    lua_pushnumber(L, ret);
}

template<>
void push(lua_State *L, char* ret)
{
    lua_pushstring(L, ret);
}

template<>
void push(lua_State *L, const char* ret)
{
    lua_pushstring(L, ret);
}

template<>
void push(lua_State *L, bool ret)
{
    lua_pushboolean(L, ret);
}

//     template<>
//     void push(lua_State *L, lua_value* ret)
//     {
//         if(ret) ret->to_lua(L); else lua_pushnil(L);
//     }

template<>
void push(lua_State *L, long long ret)
{ 
    *(long long*)lua_newuserdata(L, sizeof(long long)) = ret;
    lua_getglobal(L, "__s64");
    lua_setmetatable(L, -2);
}

template<>
void push(lua_State *L, unsigned long long ret)
{
    *(unsigned long long*)lua_newuserdata(L, sizeof(unsigned long long)) = ret;
    lua_getglobal(L, "__u64");
    lua_setmetatable(L, -2);
}

}
}
