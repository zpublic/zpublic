/********************************************************************
* @file      : tuple.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/15 15:02
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _TUPLE_HPP_
#define _TUPLE_HPP_

namespace zl
{
    class TupleNullItem
    {
    };
    template<typename T0 = TupleNullItem,typename T1 = TupleNullItem,typename T2 = TupleNullItem,typename T3 = TupleNullItem,typename T4 = TupleNullItem,typename T5 = TupleNullItem,typename T6 = TupleNullItem,typename T7 = TupleNullItem,typename T8 = TupleNullItem,typename T9 = TupleNullItem,typename T10 = TupleNullItem>
    class Tuple
    {
    };

    template<typename T0>
    class Tuple<T0>
    {
    public:
        T0 f0;

        Tuple()
        {
        }

        Tuple(T0 p0)
            :f0(p0)
        {
        }

        bool operator==(const Tuple<T0>& value)
        {
            return f0==value.f0;
        }

        bool operator!=(const Tuple<T0>& value)
        {
            return !(f0==value.f0);
        }
    };

    template<typename T0,typename T1>
    class Tuple<T0,T1>
    {
    public:
        T0 f0;T1 f1;

        Tuple()
        {
        }

        Tuple(T0 p0,T1 p1)
            :f0(p0),f1(p1)
        {
        }

        bool operator==(const Tuple<T0,T1>& value)
        {
            return f0==value.f0 && f1==value.f1;
        }

        bool operator!=(const Tuple<T0,T1>& value)
        {
            return !(f0==value.f0 && f1==value.f1);
        }
    };

    template<typename T0,typename T1,typename T2>
    class Tuple<T0,T1,T2>
    {
    public:
        T0 f0;T1 f1;T2 f2;

        Tuple()
        {
        }

        Tuple(T0 p0,T1 p1,T2 p2)
            :f0(p0),f1(p1),f2(p2)
        {
        }

        bool operator==(const Tuple<T0,T1,T2>& value)
        {
            return f0==value.f0 && f1==value.f1 && f2==value.f2;
        }

        bool operator!=(const Tuple<T0,T1,T2>& value)
        {
            return !(f0==value.f0 && f1==value.f1 && f2==value.f2);
        }
    };

    template<typename T0,typename T1,typename T2,typename T3>
    class Tuple<T0,T1,T2,T3>
    {
    public:
        T0 f0;T1 f1;T2 f2;T3 f3;

        Tuple()
        {
        }

        Tuple(T0 p0,T1 p1,T2 p2,T3 p3)
            :f0(p0),f1(p1),f2(p2),f3(p3)
        {
        }

        bool operator==(const Tuple<T0,T1,T2,T3>& value)
        {
            return f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3;
        }

        bool operator!=(const Tuple<T0,T1,T2,T3>& value)
        {
            return !(f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3);
        }
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4>
    class Tuple<T0,T1,T2,T3,T4>
    {
    public:
        T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;

        Tuple()
        {
        }

        Tuple(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
            :f0(p0),f1(p1),f2(p2),f3(p3),f4(p4)
        {
        }

        bool operator==(const Tuple<T0,T1,T2,T3,T4>& value)
        {
            return f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4;
        }

        bool operator!=(const Tuple<T0,T1,T2,T3,T4>& value)
        {
            return !(f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4);
        }
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
    class Tuple<T0,T1,T2,T3,T4,T5>
    {
    public:
        T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;

        Tuple()
        {
        }

        Tuple(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
            :f0(p0),f1(p1),f2(p2),f3(p3),f4(p4),f5(p5)
        {
        }

        bool operator==(const Tuple<T0,T1,T2,T3,T4,T5>& value)
        {
            return f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5;
        }

        bool operator!=(const Tuple<T0,T1,T2,T3,T4,T5>& value)
        {
            return !(f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5);
        }
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
    class Tuple<T0,T1,T2,T3,T4,T5,T6>
    {
    public:
        T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;T6 f6;

        Tuple()
        {
        }

        Tuple(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
            :f0(p0),f1(p1),f2(p2),f3(p3),f4(p4),f5(p5),f6(p6)
        {
        }

        bool operator==(const Tuple<T0,T1,T2,T3,T4,T5,T6>& value)
        {
            return f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5 && f6==value.f6;
        }

        bool operator!=(const Tuple<T0,T1,T2,T3,T4,T5,T6>& value)
        {
            return !(f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5 && f6==value.f6);
        }
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
    class Tuple<T0,T1,T2,T3,T4,T5,T6,T7>
    {
    public:
        T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;T6 f6;T7 f7;

        Tuple()
        {
        }

        Tuple(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
            :f0(p0),f1(p1),f2(p2),f3(p3),f4(p4),f5(p5),f6(p6),f7(p7)
        {
        }

        bool operator==(const Tuple<T0,T1,T2,T3,T4,T5,T6,T7>& value)
        {
            return f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5 && f6==value.f6 && f7==value.f7;
        }

        bool operator!=(const Tuple<T0,T1,T2,T3,T4,T5,T6,T7>& value)
        {
            return !(f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5 && f6==value.f6 && f7==value.f7);
        }
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
    class Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8>
    {
    public:
        T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;T6 f6;T7 f7;T8 f8;

        Tuple()
        {
        }

        Tuple(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
            :f0(p0),f1(p1),f2(p2),f3(p3),f4(p4),f5(p5),f6(p6),f7(p7),f8(p8)
        {
        }

        bool operator==(const Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8>& value)
        {
            return f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5 && f6==value.f6 && f7==value.f7 && f8==value.f8;
        }

        bool operator!=(const Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8>& value)
        {
            return !(f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5 && f6==value.f6 && f7==value.f7 && f8==value.f8);
        }
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
    class Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>
    {
    public:
        T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;T6 f6;T7 f7;T8 f8;T9 f9;

        Tuple()
        {
        }

        Tuple(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
            :f0(p0),f1(p1),f2(p2),f3(p3),f4(p4),f5(p5),f6(p6),f7(p7),f8(p8),f9(p9)
        {
        }

        bool operator==(const Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>& value)
        {
            return f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5 && f6==value.f6 && f7==value.f7 && f8==value.f8 && f9==value.f9;
        }

        bool operator!=(const Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>& value)
        {
            return !(f0==value.f0 && f1==value.f1 && f2==value.f2 && f3==value.f3 && f4==value.f4 && f5==value.f5 && f6==value.f6 && f7==value.f7 && f8==value.f8 && f9==value.f9);
        }
    };

}

#endif