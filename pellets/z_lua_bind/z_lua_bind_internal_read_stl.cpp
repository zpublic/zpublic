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
#include "z_lua_bind_internal_read.h"

namespace zl
{
namespace LuaBind
{

    template<>
    std::string read(lua_State *L, int index)
    {
        char* s = (char*)lua_tostring(L, index);
        if (s)
        {
            return std::string(s);
        }
        return  std::string("");
    }

    template<>
    std::list<int> read(lua_State *L, int index)
    {
        std::list<int> list;
        if (lua_istable(L, index))
        {
            int nTop = lua_gettop(L);
            int n = lua_rawlen(L, index);
            for ( int i = 0; i < n; ++i)
            {
                lua_settop(L, nTop);
                lua_rawgeti(L, index, 1 + i);
                list.push_back((int)lua_tointeger(L, -1));
            }
        }
        return list;
    }

}
}
