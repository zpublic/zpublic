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
    T upvalue_(lua_State *L)
    {
        return user2type<T>::invoke(L, lua_upvalueindex(1));
    }

    ///> 一个返回值
    template<typename RVal, typename T1=void, typename T2=void, typename T3=void, typename T4=void, typename T5=void>
    struct functor
    {
        static int invoke(lua_State *L)
        {
            push(L, upvalue_<RVal(*)(T1,T2,T3,T4,T5)>(L)(
                lua_read<T1>::r(L,1),
                lua_read<T2>::r(L,2),
                lua_read<T3>::r(L,3),
                lua_read<T4>::r(L,4),
                lua_read<T5>::r(L,5)));
            return 1;
        }
    };

    template<typename RVal, typename T1, typename T2, typename T3, typename T4>
    struct functor<RVal,T1,T2,T3,T4> 
    {
        static int invoke(lua_State *L)
        {
            push(L, upvalue_<RVal(*)(T1,T2,T3,T4)>(L)(
                lua_read<T1>::r(L,1),
                lua_read<T2>::r(L,2),
                lua_read<T3>::r(L,3),
                lua_read<T4>::r(L,4)));
            return 1;
        }
    };

    template<typename RVal, typename T1, typename T2, typename T3>
    struct functor<RVal,T1,T2,T3> 
    {
        static int invoke(lua_State *L)
        {
            push(L, upvalue_<RVal(*)(T1,T2,T3)>(L)(
                lua_read<T1>::r(L,1),
                lua_read<T2>::r(L,2),
                lua_read<T3>::r(L,3)));
            return 1;
        }
    };

    template<typename RVal, typename T1, typename T2>
    struct functor<RVal,T1,T2> 
    {
        static int invoke(lua_State *L)
        {
            push(L, upvalue_<RVal(*)(T1,T2)>(L)(
                lua_read<T1>::r(L,1),
                lua_read<T2>::r(L,2)));
            return 1;
        }
    };

    template<typename RVal, typename T1>
    struct functor<RVal,T1> 
    {
        static int invoke(lua_State *L)
        {
            push(L, upvalue_<RVal(*)(T1)>(L)(lua_read<T1>::r(L,1)));
            return 1;
        }
    };

    template<typename RVal>
    struct functor<RVal>
    {
        static int invoke(lua_State *L)
        {
            push(L, upvalue_<RVal(*)()>(L)());
            return 1;
        }
    };

    ///> 无返回值
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    struct functor<void, T1, T2, T3, T4, T5>
    {
        static int invoke(lua_State *L)
        {
            upvalue_<void(*)(T1,T2,T3,T4,T5)>(L)(
                lua_read<T1>::r(L,1),
                lua_read<T2>::r(L,2),
                lua_read<T3>::r(L,3),
                lua_read<T4>::r(L,4),
                lua_read<T5>::r(L,5));
            return 0;
        }
    };

    template<typename T1, typename T2, typename T3, typename T4>
    struct functor<void, T1, T2, T3, T4>
    {
        static int invoke(lua_State *L)
        {
            upvalue_<void(*)(T1,T2,T3,T4)>(L)(
                lua_read<T1>::r(L,1),
                lua_read<T2>::r(L,2),
                lua_read<T3>::r(L,3),
                lua_read<T4>::r(L,4));
            return 0;
        }
    };

    template<typename T1, typename T2, typename T3>
    struct functor<void, T1, T2, T3>
    {
        static int invoke(lua_State *L)
        {
            upvalue_<void(*)(T1,T2,T3)>(L)(
                lua_read<T1>::r(L,1),
                lua_read<T2>::r(L,2),
                lua_read<T3>::r(L,3));
            return 0;
        }
    };

    template<typename T1, typename T2>
    struct functor<void, T1, T2>
    {
        static int invoke(lua_State *L)
        {
            upvalue_<void(*)(T1,T2)>(L)(
                lua_read<T1>::r(L,1),
                lua_read<T2>::r(L,2));
            return 0;
        }
    };

    template<typename T1>
    struct functor<void, T1>
    {
        static int invoke(lua_State *L)
        {
            upvalue_<void(*)(T1)>(L)(lua_read<T1>::r(L,1));
            return 0;
        }
    };

    template<>
    struct functor<void>
    {
        static int invoke(lua_State *L)
        {
            upvalue_<void(*)()>(L)();
            return 0;
        }
    };

    // functor (non-managed)
    template<typename T1>
    struct functor<int, lua_State*, T1>
    {
        static int invoke(lua_State *L)
        {
            return upvalue_<int(*)(lua_State*,T1)>(L)(L,lua_read<T1>::r(L,1));
        }
    };

    template<>
    struct functor<int,lua_State*>
    {
        static int invoke(lua_State *L)
        {
            return upvalue_<int(*)(lua_State*)>(L)(L);
        }
    };

    template<typename RVal> 
    void push_functor(lua_State *L, RVal (*func)())
    {
        lua_pushcclosure(L, functor<RVal>::invoke, 1);
    }

    template<typename RVal, typename T1> 
    void push_functor(lua_State *L, RVal (*func)(T1))
    { 
        lua_pushcclosure(L, functor<RVal,T1>::invoke, 1);
    }

    template<typename RVal, typename T1, typename T2> 
    void push_functor(lua_State *L, RVal (*func)(T1,T2))
    { 
        lua_pushcclosure(L, functor<RVal,T1,T2>::invoke, 1);
    }
    
    template<typename RVal, typename T1, typename T2, typename T3> 
    void push_functor(lua_State *L, RVal (*func)(T1,T2,T3))
    { 
        lua_pushcclosure(L, functor<RVal,T1,T2,T3>::invoke, 1);
    }

    template<typename RVal, typename T1, typename T2, typename T3, typename T4> 
    void push_functor(lua_State *L, RVal (*func)(T1,T2,T3,T4))
    { 
        lua_pushcclosure(L, functor<RVal,T1,T2,T3,T4>::invoke, 1);
    }

    template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5> 
    void push_functor(lua_State *L, RVal (*func)(T1,T2,T3,T4,T5))
    { 
        lua_pushcclosure(L, functor<RVal,T1,T2,T3,T4,T5>::invoke, 1);
    }

}
}