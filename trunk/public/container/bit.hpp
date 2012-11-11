/********************************************************************
* @file      : bit.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/09 10:13
* @brief     : bitλ
* 
* 
*********************************************************************/

#ifndef _BIT_HPP_
#define _BIT_HPP_

namespace zl
{
    class Bit
    {
    public:
        Bit(int bit = 0)
        {
            Set(bit);
        }

        ~Bit()
        {

        }

    public:
        int Get() const
        {
            return m_bit;
        }

        operator int() const
        {
            return m_bit;
        }

        void Set(int bit)
        {
            m_bit = (bit == 0) ? 0 : 1;
        }

        Bit& operator = (const Bit& bit)
        {
            if(this != &bit)
            {
                m_bit = bit.Get();
            }
            return *this;
        }

        int Compare(const Bit& bit)const
        {
            if(m_bit < bit.Get())
            {
                return -1;
            }
            else if(m_bit > bit.Get())
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        bool operator == (const Bit& bit) const
        {
            return Compare(bit) == 0;
        }

        bool operator != (const Bit& bit) const
        {
            return Compare(bit) != 0;
        }

        Bit& operator &= (const Bit& bit)
        {
            if(this != &bit)
            {
                m_bit = (bit.Get() == 1 && m_bit == 1) ? 1 : 0;
            }
            return *this;
        }

        Bit& operator |= (const Bit& bit)
        {
            if(this != &bit)
            {
                m_bit = (bit.Get() == 1 || m_bit == 1) ? 1 : 0;
            }
            return *this;
        }

        Bit& operator ^= (const Bit& bit)
        {
            if(this != &bit)
            {
                m_bit = (bit != *this) ? 1 : 0;
            }
            return *this;
        }

    private:
        int m_bit;
    };

}

#endif
