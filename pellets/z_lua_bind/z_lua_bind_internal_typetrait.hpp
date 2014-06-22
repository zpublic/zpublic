#pragma once

namespace zl
{
namespace LuaBind
{

    template<typename A>
    struct base_type
    {
        typedef A type;
    };
    template<typename A>
    struct base_type<A*>
    {
        typedef A type;
    };
    template<typename A>
    struct base_type<A&>
    {
        typedef A type;
    };

    template<bool C, typename A, typename B>
    struct if_ {};
    template<typename A, typename B>
    struct if_<true, A, B>
    {
        typedef A type;
    };
    template<typename A, typename B>
    struct if_<false, A, B>
    {
        typedef B type;
    };

    template<typename A>
    struct is_ptr
    {
        static const bool value = false;
    };
    template<typename A>
    struct is_ptr<A*>
    {
        static const bool value = true;
    };
    template<typename A>
    struct is_ref
    {
        static const bool value = false;
    };
    template<typename A>
    struct is_ref<A&>
    {
        static const bool value = true;
    };

    template<typename T>
    struct void2val
    {
        static T invoke(void* input)
        {
            return *(T*)input;
        }
    };
    template<typename T>
    struct void2ptr
    {
        static T* invoke(void* input)
        {
            return (T*)input;
        }
    };
    template<typename T>
    struct void2ref
    {
        static T& invoke(void* input)
        {
            return *(T*)input;
        }
    };

    template<typename T>  
    struct void2type
    {
        static T invoke(void* ptr)
        {
            return if_
                <
                    is_ptr<T>::value,
                    void2ptr
                    <
                        typename base_type<T>::type
                    >,
                    typename if_
                    <
                        is_ref<T>::value,
                        void2ref
                        <
                            typename base_type<T>::type
                        >,
                        void2val
                        <
                            typename base_type<T>::type
                        >
                    >::type
                >::type::invoke(ptr);
        }
    };

    template<typename T>  
    struct user2type
    {
        static T invoke(lua_State *L, int index)
        {
            return void2type<T>::invoke(lua_touserdata(L, index));
        }
    };

}
}
