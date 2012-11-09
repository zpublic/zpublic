/********************************************************************
* @file      : pair.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/09 10:01
* @brief     : ถþิชื้
* 
* 
*********************************************************************/

#ifndef _PAIR_HPP_
#define _PAIR_HPP_

namespace zl
{

    template<typename K, typename V>
    class Pair
    {
    public:
        K key;
        V value;

    public:
        Pair()
        {

        }

        ~Pair()
        {

        }

        Pair(const K& _key, const V& _value)
        {
            key = _key;
            value = _value;
        }

        Pair(const Pair<K, V>& pair)
        {
            key = pair.key;
            value = pair.value;
        }

    public:
        int Compare(const Pair<K, V>& pair)const
        {
            if(key < pair.key)
            {
                return -1;
            }
            else if(key > pair.key)
            {
                return 1;
            }
            else if(value < pair.value)
            {
                return -1;
            }
            else if(value > pair.value)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        bool operator == (const Pair<K, V>& pair) const
        {
            return Compare(pair) == 0;
        }

        bool operator != (const Pair<K, V>& pair) const
        {
            return Compare(pair) != 0;
        }

        bool operator < (const Pair<K, V>& pair) const
        {
            return Compare(pair) < 0;
        }

        bool operator <= (const Pair<K, V>& pair) const
        {
            return Compare(pair) <= 0;
        }

        bool operator > (const Pair<K, V>& pair) const
        {
            return Compare(pair) > 0;
        }

        bool operator >= (const Pair<K, V>& pair) const
        {
            return Compare(pair) >= 0;
        }

    };

}

#endif