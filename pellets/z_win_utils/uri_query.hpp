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
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/

/**
 * @file
 * @brief URI(Uniform Resource Identifier)中的查询参数生成与解析相关
 */

#pragma once
#include "uri_utils.hpp"
#include <list>
#include <utility>
#include <algorithm>


namespace zl
{
namespace WinUtils
{
/**
 * @brief url中的参数列表常用操作接口, 参数列表形如"key1=value1&key2=value2"
 */
class ZLUriQuery
{
private:
    typedef std::list<std::pair<std::string,std::string> > list_type;
    list_type m_list;

public:
    typedef list_type::const_iterator const_iterator;
    typedef list_type::iterator       iterator;
    typedef list_type::value_type     value_type;

public:
    ZLUriQuery();
    ZLUriQuery(const ZLUriQuery& rhs);

    ZLUriQuery& operator= (const ZLUriQuery& rhs);

    void push_back(const std::string& sKey, const std::string& sValue);
    void push_front(const std::string& sKey, const std::string& sValue);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    void clear();
    std::string to_string();
    std::string to_raw_string();
    
private:
    iterator _find_key(const std::string& sKey);
    void     _erase_key_without_fail(const std::string& sKey);

private:
    static const std::string RESERVED_QUERY;
};

__declspec(selectany) const std::string ZLUriQuery::RESERVED_QUERY   = "#";

inline ZLUriQuery::ZLUriQuery()
{}

inline ZLUriQuery::ZLUriQuery( const ZLUriQuery& rhs )
{
    *this = rhs;
}

inline void ZLUriQuery::push_back( const std::string& sKey, const std::string& sValue )
{
    _erase_key_without_fail(sKey);
    m_list.push_back(std::make_pair(sKey, sValue));
}

inline void ZLUriQuery::push_front( const std::string& sKey, const std::string& sValue )
{
    _erase_key_without_fail(sKey);
    m_list.push_front(std::make_pair(sKey, sValue));
}

inline ZLUriQuery::iterator ZLUriQuery::_find_key( const std::string& sKey )
{
    iterator it = m_list.begin();
    for (; it!=m_list.end(); ++it)
    {
        if (it->first == sKey)
        {
            break;
        }
    }

    return it;
}

inline void ZLUriQuery::clear()
{
    m_list.clear();
}

inline std::string ZLUriQuery::to_string()
{
    return zl::WinUtils::ZLUriUtils::UrlEncode(to_raw_string(), RESERVED_QUERY);
}

inline std::string ZLUriQuery::to_raw_string()
{
    std::string s;
    for (const_iterator it=m_list.begin(); it!=m_list.end(); ++it)
    {
        if (it != m_list.begin())
            s += '&';
        s += it->first;
        s += '=';
        s += it->second;
    }
    return s;
}

inline ZLUriQuery& ZLUriQuery::operator=( const ZLUriQuery& rhs )
{
    m_list.clear();
    std::copy(rhs.m_list.begin(), rhs.m_list.end(), std::back_inserter(m_list));
    return *this;
}

inline ZLUriQuery::iterator ZLUriQuery::begin()
{
    return m_list.begin();
}

inline ZLUriQuery::const_iterator ZLUriQuery::begin() const
{
    return m_list.begin();
}

inline ZLUriQuery::iterator ZLUriQuery::end()
{
    return m_list.end();
}

inline ZLUriQuery::const_iterator ZLUriQuery::end() const
{
    return m_list.end();
}

inline void ZLUriQuery::_erase_key_without_fail( const std::string& sKey )
{
    iterator it = _find_key(sKey);
    if (it != m_list.end())
    {
        m_list.erase(it);
    }
}

} // end of namespace WinUtils
} // end of namespace zl
