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

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

namespace zl
{
namespace LuaBind
{

    template<typename T>
    struct lua_read;

    template<>
    struct lua_read<char*>
    {
        static char* r(lua_State* L, int index)
        {
            return (char*)lua_tostring(L, index);
        }
    };

    template<>
    struct lua_read<const char*>
    {
        static const char* r(lua_State* L, int index)
        {
            return (const char*)lua_tostring(L, index);
        }
    };

    template<>
    struct lua_read<char>
    {
        static char r(lua_State* L, int index)
        {
            return (char)lua_tonumber(L, index);		
        }
    };

    template<>
    struct lua_read<unsigned char>
    {
        static unsigned char r(lua_State* L, int index)
        {
            return (unsigned char)lua_tonumber(L, index);
        }
    };

    template<>
    struct lua_read<short>
    {
        static short r(lua_State* L, int index)
        {
            return (short)lua_tonumber(L, index);		
        }
    };

    template<>
    struct lua_read<unsigned short>
    {
        static unsigned short r(lua_State* L, int index)
        {
            return (unsigned short)lua_tonumber(L, index);
        }
    };

    template<>
    struct lua_read<long>
    {
        static long r(lua_State* L, int index)
        {
            return (long)lua_tonumber(L, index);		
        }
    };

    template<>
    struct lua_read<unsigned long>
    {
        static unsigned long r(lua_State* L, int index)
        {
            return (unsigned long)lua_tonumber(L, index);
        }
    };

    template<>
    struct lua_read<int>
    {
        static int r(lua_State* L, int index)
        {
            return (int)lua_tonumber(L, index);		
        }
    };

    template<>
    struct lua_read<unsigned int>
    {
        static unsigned int r(lua_State* L, int index)
        {
            return (unsigned int)lua_tonumber(L, index);
        }
    };

    template<>
    struct lua_read<float>
    {
        static float r(lua_State* L, int index)
        {
            return (float)lua_tonumber(L, index);		
        }
    };

    template<>
    struct lua_read<double>
    {
        static double r(lua_State* L, int index)
        {
            return (double)lua_tonumber(L, index);
        }
    };

    template<>
    struct lua_read<bool>
    {
        static bool r(lua_State* L, int index)
        {
            if(lua_isboolean(L, index))
                return lua_toboolean(L, index) != 0;
            else
                return lua_tonumber(L, index) != 0;	
        }
    };

    template<>
    struct lua_read<void>
    {
        static void r(lua_State* L, int index)
        {
            return;
        }
    };

    template<>
    struct lua_read<long long>
    {
        static long long r(lua_State* L, int index)
        {
            if(lua_isnumber(L,index))
                return (long long)lua_tonumber(L, index);
            else
                return *(long long*)lua_touserdata(L, index);
        }
    };

    template<>
    struct lua_read<unsigned long long>
    {
        static unsigned long long r(lua_State* L, int index)
        {
            if(lua_isnumber(L,index))
                return (unsigned long long)lua_tonumber(L, index);
            else
                return *(unsigned long long*)lua_touserdata(L, index);
        }
    };

    template<>
    struct lua_read<std::string>
    {
        static std::string r(lua_State* L, int index)
        {
            char* s = (char*)lua_tostring(L, index);
            if (s)
            {
                return std::string(s);
            }
            return std::string("");
        }
    };

    template<typename T>
    struct lua_read<std::vector<T> >
    {
        static std::vector<T> r(lua_State* L, int index)
        {
            std::vector<T> vec;
            if (lua_istable(L, index))
            {
                lua_pushnil(L);
                int real_pos = index;
                if (index < 0)
                    real_pos = real_pos - 1;
                while (lua_next(L, real_pos) != 0)
                {
                    vec.push_back(lua_read<T>::r(L, -1));
                    lua_pop(L, 1);
                }
            }
            return vec;
        }
    };

    template<typename T>
    struct lua_read<std::list<T> >
    {
        static std::list<T> r(lua_State* L, int index)
        {
            std::list<T> list;
            if (lua_istable(L, index))
            {
                lua_pushnil(L);
                int real_pos = index;
                if (index < 0)
                    real_pos = real_pos - 1;
                while (lua_next(L, real_pos) != 0)
                {
                    list.push_back(lua_read<T>::r(L, -1));
                    lua_pop(L, 1);
                }
            }
            return list;
        }
    };

    template<typename T>
    struct lua_read<std::set<T> >
    {
        static std::set<T> r(lua_State* L, int index)
        {
            std::set<T> set;
            if (lua_istable(L, index))
            {
                lua_pushnil(L);
                int real_pos = index;
                if (index < 0)
                    real_pos = real_pos - 1;
                while (lua_next(L, real_pos) != 0)
                {
                    set.insert(lua_read<T>::r(L, -1));
                    lua_pop(L, 1);
                }
            }
            return set;
        }
    };

    template<typename K, typename V>
    struct lua_read<std::map<K, V> >
    {
        static std::map<K, V> r(lua_State* L, int index)
        {
            std::map<K, V> map;

            if (lua_istable(L, index))
            {
                lua_pushnil(L);
                int real_pos = index;
                if (index < 0)
                    real_pos = real_pos - 1;
                while (lua_next(L, real_pos) != 0)
                {
                    K key = lua_read<K>::r(L, -2);
                    V val = lua_read<V>::r(L, -1);
                    map.insert(make_pair(key, val));
                    lua_pop(L, 1);
                }
            }
            return map;
        }
    };

}}
