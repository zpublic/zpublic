/********************************************************************
* @file      : string_encode.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/20 11:04
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _STRING_ENCODE_HPP_
#define _STRING_ENCODE_HPP_

namespace zl
{

    inline bool Mbs2Wide(const std::string& strMbs,
                         std::wstring& strWide,
                         unsigned codepage,
                         unsigned flags)
    {
        int count = MultiByteToWideChar(codepage, 0, strMbs.data(), strMbs.length(), NULL, 0);
        if (count > 0)
        {
            strWide.resize(count);
            count = MultiByteToWideChar(codepage, flags, strMbs.data(), strMbs.length(),
                const_cast<wchar_t*>(strWide.data()), strWide.length());
        }
        return count > 0;
    }


    inline bool Wide2Mbs(const std::wstring& strWide,
                         std::string& strMbs,
                         unsigned codepage,
                         unsigned flags,
                         bool useDefault)
    {
        BOOL bUseDefault = FALSE;
        int count = WideCharToMultiByte(codepage, 0, strWide.data(), strWide.length(), 
            NULL, 0, NULL, NULL);
        if (count > 0)
        {
            strMbs.resize(count);
            BOOL* pUseDefault = (useDefault ? &bUseDefault : NULL);
            count = WideCharToMultiByte(codepage, flags, strWide.data(), strWide.length(),
                const_cast<char*>(strMbs.data()), strMbs.length(), NULL, pUseDefault);
        }
        return (count > 0 && !bUseDefault);
    }


    inline std::wstring UTF8ToWide(const std::string &utf8)
    {
        std::wstring strWide;
        if (!Mbs2Wide(utf8, strWide, CP_UTF8, 0))
        {
            strWide.clear();
        }
        return strWide;
    }

    inline std::string WideToUTF8(const std::wstring &wide)
    {
        std::string strUtf8;
        if (!Wide2Mbs(wide, strUtf8, CP_UTF8, 0, false))
        {
            strUtf8.clear();
        }
        return strUtf8;
    }

    inline std::string WideToGbk(const std::wstring& strWide)
    {
        std::string strGbk;
        if (!Wide2Mbs(strWide, strGbk, 936, 0, true)) // CP_GB2312 = 936
        {
            strGbk.clear();
        }
        return strGbk;
    }

    inline std::wstring GbkToWide(const std::string& strGbk)
    {
        std::wstring strWide;
        if (!Mbs2Wide(strGbk, strWide, 936, 0)) // CP_GB2312 = 936
        {
            strWide.clear();
        }
        return strWide;
    }

    inline std::string Utf8ToGbk(const std::string& strUtf8)
    {
        return WideToGbk(UTF8ToWide(strUtf8));
    }

    inline std::string GbkToUtf8(const std::string& strGbk)
    {
        return WideToUTF8(GbkToWide(strGbk));
    }
}

#endif