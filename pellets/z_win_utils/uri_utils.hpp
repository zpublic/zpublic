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
 * @brief URI(Uniform Resource Identifier)常用的工具
 * 
 * 如urlencode/urldecode等
 */


#pragma once

namespace zl
{
namespace WinUtils
{

/**
 * @brief url编码相关
 * 
 * URLEncode是这样编码的 
 * 1. 数字和字母不变。
 * 2. 空格变为"+"号。
 * 3. 其他被编码成"%"加上他们的ascii的十六进制，规律是这样的 
 *    比如“啊”字 Ascii的十六进制是B0A1——>%B0%A1(Note:它是每个字节前加个%)。
 * 4. 绝对不编码的，只有字母、数字、短横线(-)、下划线(_)、点(.)和波浪号(~)
 * ，其他字符要视情况而定，所以一般性的urlencode只需保留上述字符不进行编码。
 */
class ZLUriUtils
{
public:
    /**
     * @brief url编码
     * @param[in] str 要编码的字符串
     * @param[in] reserved需要保留无需进行编码的字符集
     * @return url编码后的字符串
     */
    static std::string UrlEncode(const std::string& str, const std::string& reserved);
    static std::string UrlDecode(const std::string& str);

private:
    static unsigned char ZLUriUtils::_ToHex( unsigned char x );
    static unsigned char ZLUriUtils::_FromHex( unsigned char x );

    static const std::string s_sMustBeEncode;
};


__declspec(selectany) const std::string ZLUriUtils::s_sMustBeEncode = "%<>{}|\\\"^`";

inline std::string ZLUriUtils::UrlEncode( const std::string& str, const std::string& reserved )
{
    std::string sEncodedStr;

    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        char c = *it;
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '-' || c == '_' ||
            c == '.' || c == '~')
        {
            sEncodedStr += c;
        }
        else if (c <= 0x20 || c >= 0x7F || s_sMustBeEncode.find(c) != std::string::npos || reserved.find(c) != std::string::npos)
        {
            sEncodedStr += '%';
            sEncodedStr += _ToHex((unsigned char)c >> 4);
            sEncodedStr += _ToHex((unsigned char)c % 16);
        }
        else sEncodedStr += c;
    }

    return sEncodedStr;
}

inline std::string ZLUriUtils::UrlDecode( const std::string& str )
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            if (i+2 < length)
            {
                unsigned char high = _FromHex((unsigned char)str[++i]);
                unsigned char low  = _FromHex((unsigned char)str[++i]);
                strTemp += high*16 + low;
            }
        }
        else strTemp += str[i];
    }
    return strTemp;
}

inline unsigned char ZLUriUtils::_ToHex( unsigned char x )
{
    return  x > 9 ? x + 55 : x + 48;
}

inline unsigned char ZLUriUtils::_FromHex( unsigned char x )
{
    unsigned char y = 0;
    if (x >= 'A' && x <= 'Z')       y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z')  y = x - 'a' + 10;
    else if (x >= '0' && x <= '9')  y = x - '0';
    else y = 0;
    return y;
}

} // end of namespace WinUtils
} // end of namespace zl
