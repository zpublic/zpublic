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

namespace zl
{
namespace LuaBind
{

    template<typename T>  
    T read(lua_State *L, int index)
    {
        ///> 暂不支持复杂类型
        do { typedef int ai[1]; } while(0);
        T x;
        return x;
    }

    template<>	char*               read(lua_State *L, int index);
    template<>	const char*         read(lua_State *L, int index);
    template<>	char                read(lua_State *L, int index);
    template<>	unsigned char       read(lua_State *L, int index);
    template<>	short               read(lua_State *L, int index);
    template<>	unsigned short      read(lua_State *L, int index);
    template<>	long                read(lua_State *L, int index);
    template<>	unsigned long       read(lua_State *L, int index);
    template<>	int                 read(lua_State *L, int index);
    template<>	unsigned int        read(lua_State *L, int index);
    template<>	float               read(lua_State *L, int index);
    template<>	double              read(lua_State *L, int index);
    template<>	bool                read(lua_State *L, int index);
    template<>	void                read(lua_State *L, int index);
    template<>	long long           read(lua_State *L, int index);
    template<>	unsigned long long  read(lua_State *L, int index);

}}
