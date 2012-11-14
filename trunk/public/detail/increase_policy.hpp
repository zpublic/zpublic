/********************************************************************
* @file      : increase_policy.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/14 15:32
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _INCREASE_POLICY_
#define _INCREASE_POLICY_

namespace zl
{

    class IIncrease
    {
    public:
        IIncrease() : m_Num(0) {}
        ~IIncrease() {}

    public:
        void Set(unsigned long num) { m_Num = num; }
        virtual unsigned Increase() = 0;

    protected:
        unsigned long m_Num;
    };

    template <unsigned long N>
    class PIncreaseRegular : public IIncrease
    {
    public:
        virtual unsigned Increase()
        {
            m_Num += N;
            return m_Num;
        }
    };

    template <unsigned long N>
    class PIncreaseMul : public IIncrease
    {
    public:
        virtual unsigned Increase()
        {
            m_Num *= N;
            return m_Num;
        }
    };

}

#endif