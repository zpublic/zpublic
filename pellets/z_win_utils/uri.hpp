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
 * @brief URI(Uniform Resource Identifier)相关
 *
 *  uri生成与解析相关操作接口
 *      
 *      Code of this file is based on Poco library, file: URI.h.
 *      Original copyright is included below.
 *      Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
 *      and Contributors.
 */


#pragma once
#include <string>
#include <vector>
#include "uri_utils.hpp"

namespace zl
{
namespace WinUtils
{

/**
 * @brief uri生成与解析
 * 
 * @code
 * Original URI   : http://127.0.0.1:611/index.html?a=1&var2=%20aaa%20%A
 * getHost        : 127.0.0.1
 * getPort        : 611
 * toString       : http://127.0.0.1:611/index.html?a=1&var2=%20aaa%20%A
 * getScheme      : http
 * getUserInfo    :
 * getPath        : /index.html
 * isRelative     : 0
 * getAuthority   : 127.0.0.1:611
 * getQuery       : a=1&var2= aaa
 * getRawQuery    : a=1&var2=%20aaa%20%A
 * getFragment    :
 * getPathEtc     : /index.html?a=1&var2=%20aaa%20%A
 * getPathAndQuery: /index.html?a=1&var2=%20aaa%20%A
 * empty          : 0
 * isIPAddress    : 1
 * getPortAsString: 611
 * @endcode
 */
class ZLUri
{
public:
    ZLUri();
    explicit ZLUri(const std::string& uri);
    explicit ZLUri(const char* uri);
    ZLUri(const std::string& scheme, const std::string& pathEtc);
    ZLUri(const std::string& scheme, const std::string& authority, const std::string& pathEtc);
    ZLUri(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query);
    ZLUri(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query, const std::string& fragment);
    ZLUri(const ZLUri& uri);
    ZLUri(const ZLUri& baseURI, const std::string& relativeURI);
    ~ZLUri();

    ZLUri& operator = (const ZLUri& uri);
    ZLUri& operator = (const std::string& uri);
    ZLUri& operator = (const char* uri);

    void Swap(ZLUri& rhs);
    void Clear();
    std::string         ToString() const;

    void SetScheme(const std::string& scheme);
    void SetUserInfo(const std::string& userInfo);
    void SetAuthority(const std::string& authority);
    void SetHost(const std::string& host);
    void SetPort(unsigned short port);
    void SetPath(const std::string& path);
    void SetQuery(const std::string& query);
    void SetFragment(const std::string& fragment);
    void SetRawQuery(const std::string& query);
    void SetPathEtc(const std::string& pathEtc);

    unsigned short GetPort() const;
    std::string GetScheme() const;
    std::string GetUserInfo() const;
    std::string GetHost() const;
    std::string GetPortAsString() const;
    std::string GetAuthority() const;
    std::string GetPath() const;
    std::string GetQuery() const;
    std::string GetRawQuery() const;
    std::string GetFragment() const;
    std::string GetPathEtc() const;
    std::string GetPathAndQuery() const;

    void Resolve(const std::string& relativeURI);
    void Resolve(const ZLUri& relativeURI);
    bool IsRelative() const;
    bool IsEmpty() const;
    bool operator == (const ZLUri& uri) const;
    bool operator == (const std::string& uri) const;
    bool operator != (const ZLUri& uri) const;
    bool operator != (const std::string& uri) const;
    void Normalize();
    void _GetPathSegments(std::vector<std::string>& segments);
    static bool IsIPAddress( const std::string& str );
    void SetPrintErrors( bool in );

private:
    bool _Equals(const ZLUri& uri) const;
    bool _IsWellKnownPort() const;
    unsigned short _GetWellKnownPort() const;
    void _Parse(const std::string& uri);
    void _ParseAuthority(std::string::const_iterator& it, const std::string::const_iterator& end);
    void _ParseHostAndPort(std::string::const_iterator& it, const std::string::const_iterator& end);
    void _ParsePath(std::string::const_iterator& it, const std::string::const_iterator& end);
    void _ParsePathEtc(std::string::const_iterator& it, const std::string::const_iterator& end);
    void _ParseQuery(std::string::const_iterator& it, const std::string::const_iterator& end);
    void _ParseFragment(std::string::const_iterator& it, const std::string::const_iterator& end);
    void _MergePath(const std::string& path);
    void _RemoveDotSegments(bool removeLeading = true);
    static void _GetPathSegments(const std::string& path, std::vector<std::string>& segments);
    void _BuildPath(const std::vector<std::string>& segments, bool leadingSlash, bool trailingSlash);

    static const std::string RESERVED_PATH;
    static const std::string RESERVED_QUERY;
    static const std::string RESERVED_FRAGMENT;

private:
    static std::string intToStr( int a );
    // Replaces all characters in str with their lower-case counterparts.
    template <class S>
    S& toLowerInPlace3(S& str)
    {
        typename S::iterator it  = str.begin();
        typename S::iterator end = str.end();
        // originally Ascii::toLower() was used.
        while (it != end) { *it = tolower(*it); ++it; }
        return str;
    }

private:
    std::string    m_pScheme;
    std::string    m_pUserInfo;
    std::string    m_pHost;
    unsigned short m_uPort;
    std::string    m_pPortStr;
    std::string    m_pPath;
    std::string    m_pQuery;
    std::string    m_pFragment;
    long           m_pFlags;
};


// 静态成员变量初始化
__declspec(selectany) const std::string ZLUri::RESERVED_PATH        = "?#";
__declspec(selectany) const std::string ZLUri::RESERVED_QUERY       = "#";
__declspec(selectany) const std::string ZLUri::RESERVED_FRAGMENT    = "";

// 实现部分
inline ZLUri::ZLUri() : m_uPort(0), m_pFlags(0) {}

inline ZLUri::ZLUri( const std::string& uri ) : m_uPort(0), m_pFlags(0) { _Parse(uri); }

inline ZLUri::ZLUri( const char* uri ) : m_uPort(0), m_pFlags(0)
{
    if (uri) _Parse(std::string(uri));
}

inline ZLUri::ZLUri( const std::string& scheme, const std::string& pathEtc )
    : m_pScheme(scheme), m_uPort(0), m_pFlags(0)
{
    toLowerInPlace3(m_pScheme);
    m_uPort = _GetWellKnownPort();
    //_portStr = intToStr(_port);	//rm_a
    std::string::const_iterator beg = pathEtc.begin();
    std::string::const_iterator end = pathEtc.end();
    _ParsePathEtc(beg, end);
}

inline ZLUri::ZLUri( const std::string& scheme, const std::string& authority, const std::string& pathEtc )
    : m_pScheme(scheme)
{
    toLowerInPlace3(m_pScheme);
    std::string::const_iterator beg = authority.begin();
    std::string::const_iterator end = authority.end();
    _ParseAuthority(beg, end);
    beg = pathEtc.begin();
    end = pathEtc.end();
    _ParsePathEtc(beg, end);
}

inline ZLUri::ZLUri( const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query )
    : m_pScheme(scheme), m_pPath(path), m_pQuery(query)
{
    toLowerInPlace3(m_pScheme);
    std::string::const_iterator beg = authority.begin();
    std::string::const_iterator end = authority.end();
    _ParseAuthority(beg, end);
}

inline ZLUri::ZLUri( const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query, const std::string& fragment )
    : m_pScheme(scheme), m_pPath(path), m_pQuery(query), m_pFragment(fragment)
{
    toLowerInPlace3(m_pScheme);
    std::string::const_iterator beg = authority.begin();
    std::string::const_iterator end = authority.end();
    _ParseAuthority(beg, end);
}

inline ZLUri::ZLUri( const ZLUri& uri )
    :
    m_pScheme(uri.m_pScheme),
    m_pUserInfo(uri.m_pUserInfo),
    m_pHost(uri.m_pHost),
    m_uPort(uri.m_uPort),
    m_pPortStr(uri.m_pPortStr),
    m_pPath(uri.m_pPath),
    m_pQuery(uri.m_pQuery),
    m_pFragment(uri.m_pFragment),
    m_pFlags(0)
{
}

inline ZLUri::ZLUri( const ZLUri& baseURI, const std::string& relativeURI )
    :
    m_pScheme(baseURI.m_pScheme),
    m_pUserInfo(baseURI.m_pUserInfo),
    m_pHost(baseURI.m_pHost),
    m_uPort(baseURI.m_uPort),
    m_pPortStr(baseURI.m_pPortStr),
    m_pPath(baseURI.m_pPath),
    m_pQuery(baseURI.m_pQuery),
    m_pFragment(baseURI.m_pFragment),
    m_pFlags(0)
{
    Resolve(relativeURI);
}

inline ZLUri::~ZLUri()
{

}

inline ZLUri& ZLUri::operator = (const ZLUri& uri)
{
    if (&uri != this)
    {
        m_pScheme   = uri.m_pScheme;
        m_pUserInfo = uri.m_pUserInfo;
        m_pHost     = uri.m_pHost;
        m_uPort     = uri.m_uPort;
        m_pPortStr  = uri.m_pPortStr;
        m_pPath     = uri.m_pPath;
        m_pQuery    = uri.m_pQuery;
        m_pFragment = uri.m_pFragment;
        m_pFlags    = uri.m_pFlags;
    }
    return *this;
}

inline ZLUri& ZLUri::operator = (const std::string& uri)
{
    Clear();
    _Parse(uri);
    return *this;
}

inline ZLUri& ZLUri::operator = (const char* uri)
{
    Clear();
    if (uri)
    {
        _Parse(std::string(uri));
    }
    return *this;
}

inline void ZLUri::Swap( ZLUri& rhs )
{
    std::swap(m_pScheme, rhs.m_pScheme);
    std::swap(m_pUserInfo, rhs.m_pUserInfo);
    std::swap(m_pHost, rhs.m_pHost);
    std::swap(m_uPort, rhs.m_uPort);
    std::swap(m_pPortStr, rhs.m_pPortStr);
    std::swap(m_pPath, rhs.m_pPath);
    std::swap(m_pQuery, rhs.m_pQuery);
    std::swap(m_pFragment, rhs.m_pFragment);
    std::swap(m_pFlags, rhs.m_pFlags);
}

inline void ZLUri::Clear()
{
    m_pScheme.clear();
    m_pUserInfo.clear();
    m_pHost.clear();
    m_uPort = 0;
    m_pPortStr.clear();
    m_pPath.clear();
    m_pQuery.clear();
    m_pFragment.clear();
    m_pFlags = 0;
}

inline std::string ZLUri::ToString() const
{
    std::string uri;
    if (IsRelative())
    {
        uri += zl::WinUtils::ZLUriUtils::UrlEncode(m_pPath, RESERVED_PATH);
    }
    else
    {
        uri = m_pScheme;
        uri += ':';
        std::string auth = GetAuthority();
        if (!auth.empty() || m_pScheme == "file")
        {
            uri.append("//");
            uri.append(auth);
        }
        if (!m_pPath.empty())
        {
            if (!auth.empty() && m_pPath[0] != '/')
                uri += '/';
            uri += zl::WinUtils::ZLUriUtils::UrlEncode(m_pPath, RESERVED_PATH);
        }
        else if (!m_pQuery.empty() || !m_pFragment.empty())
        {
            uri += '/';
        }
    }
    if (!m_pQuery.empty())
    {
        uri += '?';
        uri.append(m_pQuery);
    }
    if (!m_pFragment.empty())
    {
        uri += '#';
        uri += zl::WinUtils::ZLUriUtils::UrlEncode(m_pFragment, RESERVED_FRAGMENT);
    }
    return uri;
}

inline void ZLUri::SetScheme(const std::string& scheme)
{
    m_pScheme = scheme;
    toLowerInPlace3(m_pScheme);
    if (m_uPort == 0){
        m_uPort = _GetWellKnownPort();
        //_portStr = intToStr(_port);	//rm_a
    }
}

inline void ZLUri::SetUserInfo(const std::string& userInfo)
{
    m_pUserInfo = zl::WinUtils::ZLUriUtils::UrlDecode(userInfo);
}


inline void ZLUri::SetHost(const std::string& host)
{
    m_pHost = host;
}

inline unsigned short ZLUri::GetPort() const
{
    if (m_uPort == 0)
        return _GetWellKnownPort();
    else
        return m_uPort;
}

inline std::string ZLUri::GetPortAsString() const
{
    return m_pPortStr;
}

inline void ZLUri::SetPort(unsigned short port)
{
    m_uPort = port;
    m_pPortStr = intToStr(port);
}

inline std::string ZLUri::GetAuthority() const
{
    std::string auth;
    if (!m_pUserInfo.empty())
    {
        auth.append(m_pUserInfo);
        auth += '@';
    }
    if (m_pHost.find(':') != std::string::npos)
    {
        auth += '[';
        auth += m_pHost;
        auth += ']';
    }
    else auth.append(m_pHost);
    if (m_uPort && !_IsWellKnownPort())
    {
        auth += ':';
        auth += intToStr(m_uPort);
    }
    return auth;
}

inline void ZLUri::SetAuthority(const std::string& authority)
{
    m_pUserInfo.clear();
    m_pHost.clear();
    m_uPort = 0;
    m_pPortStr.clear();	// instead of ="0";
    std::string::const_iterator beg = authority.begin();
    std::string::const_iterator end = authority.end();
    _ParseAuthority(beg, end);
}

inline void ZLUri::SetPath(const std::string& path)
{
    m_pPath = zl::WinUtils::ZLUriUtils::UrlDecode(path);
}


inline void ZLUri::SetRawQuery(const std::string& query)
{
    m_pQuery = query;
}

inline void ZLUri::SetQuery(const std::string& query)
{
    m_pQuery = zl::WinUtils::ZLUriUtils::UrlEncode(query, RESERVED_QUERY);
}

inline std::string ZLUri::GetQuery() const
{
    return zl::WinUtils::ZLUriUtils::UrlDecode(m_pQuery);
}

inline void ZLUri::SetFragment(const std::string& fragment)
{
    m_pFragment = zl::WinUtils::ZLUriUtils::UrlDecode(fragment);
}

inline void ZLUri::SetPathEtc(const std::string& pathEtc)
{
    m_pPath.clear();
    m_pQuery.clear();
    m_pFragment.clear();
    std::string::const_iterator beg = pathEtc.begin();
    std::string::const_iterator end = pathEtc.end();
    _ParsePathEtc(beg, end);
}

inline std::string ZLUri::GetPathEtc() const
{
    std::string pathEtc = zl::WinUtils::ZLUriUtils::UrlEncode(m_pPath, RESERVED_PATH);
    if (!m_pQuery.empty())
    {
        pathEtc += '?';
        pathEtc += m_pQuery;
    }
    if (!m_pFragment.empty())
    {
        pathEtc += '#';
        pathEtc += zl::WinUtils::ZLUriUtils::UrlEncode(m_pFragment, RESERVED_FRAGMENT);
    }
    return pathEtc;
}

inline std::string ZLUri::GetPathAndQuery() const
{
    std::string pathAndQuery = zl::WinUtils::ZLUriUtils::UrlEncode(m_pPath, RESERVED_PATH);
    if (!m_pQuery.empty())
    {
        pathAndQuery += '?';
        pathAndQuery += m_pQuery;
    }
    return pathAndQuery;
}

inline void ZLUri::Resolve(const std::string& relativeURI)
{
    ZLUri parsedURI(relativeURI);
    Resolve(parsedURI);
}

inline void ZLUri::Resolve(const ZLUri& relativeURI)
{
    if (!relativeURI.m_pScheme.empty())
    {
        m_pScheme   = relativeURI.m_pScheme;
        m_pUserInfo = relativeURI.m_pUserInfo;
        m_pHost     = relativeURI.m_pHost;
        m_uPort     = relativeURI.m_uPort;
        m_pPortStr  = relativeURI.m_pPortStr;
        m_pPath     = relativeURI.m_pPath;
        m_pQuery    = relativeURI.m_pQuery;
        m_pFlags    = relativeURI.m_pFlags;
        _RemoveDotSegments();
    }
    else
    {
        if (!relativeURI.m_pHost.empty())
        {
            m_pUserInfo = relativeURI.m_pUserInfo;
            m_pHost     = relativeURI.m_pHost;
            m_uPort     = relativeURI.m_uPort;
            m_pPortStr  = relativeURI.m_pPortStr;
            m_pPath     = relativeURI.m_pPath;
            m_pQuery    = relativeURI.m_pQuery;
            m_pFlags    = relativeURI.m_pFlags;
            _RemoveDotSegments();
        }
        else
        {
            if (relativeURI.m_pPath.empty())
            {
                if (!relativeURI.m_pQuery.empty())
                    m_pQuery = relativeURI.m_pQuery;
            }
            else
            {
                if (relativeURI.m_pPath[0] == '/')
                {
                    m_pPath = relativeURI.m_pPath;
                    _RemoveDotSegments();
                }
                else
                {
                    _MergePath(relativeURI.m_pPath);
                }
                m_pQuery = relativeURI.m_pQuery;
            }
        }
    }
    m_pFragment = relativeURI.m_pFragment;
}

inline bool ZLUri::IsRelative() const
{
    return m_pScheme.empty();
}

inline bool ZLUri::IsEmpty() const
{
    return m_pScheme.empty() && m_pHost.empty() && m_pPath.empty() && m_pQuery.empty() && m_pFragment.empty();
}

inline bool ZLUri::operator == (const ZLUri& uri) const
{
    return _Equals(uri);
}

inline bool ZLUri::operator == (const std::string& uri) const
{
    ZLUri parsedURI(uri);
    return _Equals(parsedURI);
}

inline bool ZLUri::operator != (const ZLUri& uri) const
{
    return !_Equals(uri);
}

inline bool ZLUri::operator != (const std::string& uri) const
{
    ZLUri parsedURI(uri);
    return !_Equals(parsedURI);
}

inline bool ZLUri::_Equals(const ZLUri& uri) const
{
    return m_pScheme   == uri.m_pScheme
        && m_pUserInfo == uri.m_pUserInfo
        && m_pHost     == uri.m_pHost
        && GetPort() == uri.GetPort()
        && m_pPath     == uri.m_pPath
        && m_pQuery    == uri.m_pQuery
        && m_pFragment == uri.m_pFragment;
}

inline void ZLUri::Normalize()
{
    _RemoveDotSegments(!IsRelative());
}

inline void ZLUri::_RemoveDotSegments(bool removeLeading)
{
    if (m_pPath.empty()) return;

    bool leadingSlash  = *(m_pPath.begin()) == '/';
    bool trailingSlash = *(m_pPath.rbegin()) == '/';
    std::vector<std::string> segments;
    std::vector<std::string> normalizedSegments;
    _GetPathSegments(segments);
    for (std::vector<std::string>::const_iterator it = segments.begin(); it != segments.end(); ++it)
    {
        if (*it == "..")
        {
            if (!normalizedSegments.empty())
            {
                if (normalizedSegments.back() == "..")
                    normalizedSegments.push_back(*it);
                else
                    normalizedSegments.pop_back();
            }
            else if (!removeLeading)
            {
                normalizedSegments.push_back(*it);
            }
        }
        else if (*it != ".")
        {
            normalizedSegments.push_back(*it);
        }
    }
    _BuildPath(normalizedSegments, leadingSlash, trailingSlash);
}

inline void ZLUri::_GetPathSegments(std::vector<std::string>& segments)
{
    _GetPathSegments(m_pPath, segments);
}

inline void ZLUri::_GetPathSegments(const std::string& path, std::vector<std::string>& segments)
{
    std::string::const_iterator it  = path.begin();
    std::string::const_iterator end = path.end();
    std::string seg;
    while (it != end)
    {
        if (*it == '/')
        {
            if (!seg.empty())
            {
                segments.push_back(seg);
                seg.clear();
            }
        }
        else seg += *it;
        ++it;
    }
    if (!seg.empty())
        segments.push_back(seg);
}

inline bool ZLUri::_IsWellKnownPort() const
{
    return m_uPort == _GetWellKnownPort();
}

inline unsigned short ZLUri::_GetWellKnownPort() const
{
    if (m_pScheme == "ftp")
        return 21;
    else if (m_pScheme == "ssh")
        return 22;
    else if (m_pScheme == "telnet")
        return 23;
    else if (m_pScheme == "http")
        return 80;
    else if (m_pScheme == "nntp")
        return 119;
    else if (m_pScheme == "ldap")
        return 389;
    else if (m_pScheme == "https")
        return 443;
    else if (m_pScheme == "rtsp")
        return 554;
    else if (m_pScheme == "sip")
        return 5060;
    else if (m_pScheme == "sips")
        return 5061;
    else if (m_pScheme == "xmpp")
        return 5222;
    else
        return 0;
}

inline void ZLUri::_Parse(const std::string& uri)
{
    std::string::const_iterator it  = uri.begin();
    std::string::const_iterator end = uri.end();
    if (it == end) return;
    if (*it != '/' && *it != '.' && *it != '?' && *it != '#')
    {
        std::string scheme;
        while (it != end && *it != ':' && *it != '?' && *it != '#' && *it != '/') scheme += *it++;
        if (it != end && *it == ':')
        {
            ++it;
            //			if (it == end) throw SyntaxException("URI scheme must be followed by authority or path", uri);
            if (it == end){
                if( m_pFlags & 0x1 )
                    printf("ERROR: URI scheme must be followed by authority or path.\n");
                return;
            }
            SetScheme(scheme);
            if (*it == '/')
            {
                ++it;
                if (it != end && *it == '/')
                {
                    ++it;
                    _ParseAuthority(it, end);
                }
                else --it;
            }
            _ParsePathEtc(it, end);
        }
        else
        {
            it = uri.begin();
            _ParsePathEtc(it, end);
        }
    }
    else _ParsePathEtc(it, end);
}

inline void ZLUri::_ParseAuthority(std::string::const_iterator& it, const std::string::const_iterator& end)
{
    std::string userInfo;
    std::string part;
    while (it != end && *it != '/' && *it != '?' && *it != '#')
    {
        if (*it == '@')
        {
            userInfo = part;
            part.clear();
        }
        else part += *it;
        ++it;
    }
    std::string::const_iterator pbeg = part.begin();
    std::string::const_iterator pend = part.end();
    _ParseHostAndPort(pbeg, pend);
    m_pUserInfo = userInfo;
}

inline void ZLUri::_ParseHostAndPort(std::string::const_iterator& it, const std::string::const_iterator& end)
{
    if (it == end) return;
    std::string host;
    if (*it == '[')
    {
        // IPv6 address
        ++it;
        while (it != end && *it != ']') host += *it++;
        //		if (it == end) throw SyntaxException("unterminated IPv6 address");
        if (it == end){
            if( m_pFlags & 0x1 )
                printf("ERROR: unterminated IPv6 address.\n");
            return;
        }
        ++it;
    }
    else
    {
        while (it != end && *it != ':') host += *it++;
    }
    if (it != end && *it == ':')
    {
        ++it;
        std::string port;
        while (it != end) port += *it++;
        if (!port.empty())
        {
            int nport = 0;
            //if (NumberParser::tryParse(port, nport) && nport > 0 && nport < 65536)
            //	_port = (unsigned short) nport;
            //else{
            //	throw SyntaxException("bad or invalid port number", port);
            //}
            m_pPortStr = port.c_str();
            nport = atoi( port.c_str() );
            if( nport < 0 ){
                m_uPort = 0;
            }else if( nport > 65535 ){
                m_uPort = 65535;
            }else{
                m_uPort = (unsigned short) nport;
            }
        }else{
            m_uPort = _GetWellKnownPort();
            //_portStr = intToStr( _port );	//rm_a
        }
    }else{
        m_uPort = _GetWellKnownPort();
        //_portStr = intToStr(_port);	//rm_a
    }
    m_pHost = host;
    toLowerInPlace3(m_pHost);
}

inline void ZLUri::_ParsePath(std::string::const_iterator& it, const std::string::const_iterator& end)
{
    std::string path;
    while (it != end && *it != '?' && *it != '#') path += *it++;
    m_pPath = zl::WinUtils::ZLUriUtils::UrlDecode(path);
}

inline void ZLUri::_ParsePathEtc(std::string::const_iterator& it, const std::string::const_iterator& end)
{
    if (it == end) return;
    if (*it != '?' && *it != '#')
        _ParsePath(it, end);
    if (it != end && *it == '?')
    {
        ++it;
        _ParseQuery(it, end);
    }
    if (it != end && *it == '#')
    {
        ++it;
        _ParseFragment(it, end);
    }
}

inline void ZLUri::_ParseQuery(std::string::const_iterator& it, const std::string::const_iterator& end)
{
    m_pQuery.clear();
    while (it != end && *it != '#') m_pQuery += *it++;
}

inline void ZLUri::_ParseFragment(std::string::const_iterator& it, const std::string::const_iterator& end)
{
    std::string fragment;
    while (it != end) fragment += *it++;
    m_pFragment = zl::WinUtils::ZLUriUtils::UrlDecode(fragment);
}

inline void ZLUri::_MergePath(const std::string& path)
{
    std::vector<std::string> segments;
    std::vector<std::string> normalizedSegments;
    bool addLeadingSlash = false;
    if (!m_pPath.empty())
    {
        _GetPathSegments(segments);
        bool endsWithSlash = *(m_pPath.rbegin()) == '/';
        if (!endsWithSlash && !segments.empty())
            segments.pop_back();
        addLeadingSlash = m_pPath[0] == '/';
    }
    _GetPathSegments(path, segments);
    addLeadingSlash = addLeadingSlash || (!path.empty() && path[0] == '/');
    bool hasTrailingSlash = (!path.empty() && *(path.rbegin()) == '/');
    bool addTrailingSlash = false;
    for (std::vector<std::string>::const_iterator it = segments.begin(); it != segments.end(); ++it)
    {
        if (*it == "..")
        {
            addTrailingSlash = true;
            if (!normalizedSegments.empty())
                normalizedSegments.pop_back();
        }
        else if (*it != ".")
        {
            addTrailingSlash = false;
            normalizedSegments.push_back(*it);
        }
        else addTrailingSlash = true;
    }
    _BuildPath(normalizedSegments, addLeadingSlash, hasTrailingSlash || addTrailingSlash);
}

inline void ZLUri::_BuildPath(const std::vector<std::string>& segments, bool leadingSlash, bool trailingSlash)
{
    m_pPath.clear();
    bool first = true;
    for (std::vector<std::string>::const_iterator it = segments.begin(); it != segments.end(); ++it)
    {
        if (first)
        {
            first = false;
            if (leadingSlash)
                m_pPath += '/';
            else if (m_pScheme.empty() && (*it).find(':') != std::string::npos)
                m_pPath.append("./");
        }
        else m_pPath += '/';
        m_pPath.append(*it);
    }
    if (trailingSlash)
        m_pPath += '/';
}

inline bool ZLUri::IsIPAddress( const std::string& str )
{
    const char* str2 = str.c_str();
    const char* ptr = strrchr( str2, (int)'.' );
    if( ptr && *ptr == '.' ){
        ++ptr;
        if( strchr("0123456789", *ptr ) )
            return 1;
    }
    return 0;
}

inline void ZLUri::SetPrintErrors( bool in )
{
    if(in){
        m_pFlags |= 0x1;
    }else{
        m_pFlags &= ~0x1;
    }
}

inline std::string ZLUri::intToStr( int a )
{
    char bfr[32];
    sprintf_s(bfr,"%d",a);
    return bfr;
}

inline std::string ZLUri::GetScheme() const
{
    return m_pScheme;
}

inline std::string ZLUri::GetUserInfo() const
{
    return m_pUserInfo;
}

inline std::string ZLUri::GetHost() const
{
    return m_pHost;
}

inline std::string ZLUri::GetPath() const
{
    return m_pPath;
}

inline std::string ZLUri::GetRawQuery() const
{
    return m_pQuery;
}

inline std::string ZLUri::GetFragment() const
{
    return m_pFragment;
}

inline void swap(ZLUri& u1, ZLUri& u2)
{
    u1.Swap(u2);
}


} // end of namespace WinUtils
} // end of namespace zl
