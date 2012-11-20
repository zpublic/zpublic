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

    std::wstring UTF8ToWide(const std::string &utf8)
    {
        if (utf8.length() == 0)
        {
            return std::wstring();
        }

        int charcount = MultiByteToWideChar(
            CP_UTF8,
            0,
            utf8.c_str(),
            -1,
            NULL,
            0);

        if (charcount == 0)
        {
            return std::wstring();
        }

        wchar_t* buf = new wchar_t[charcount];
        MultiByteToWideChar(
            CP_UTF8,
            0,
            utf8.c_str(),
            -1,
            buf,
            charcount);
        std::wstring result(buf);
        delete[] buf;
        return result;
    }

    std::string WideToUTF8(const std::wstring &wide)
    {
        if (wide.length() == 0)
        {
            return std::string();
        }

        int charcount = WideCharToMultiByte(
            CP_UTF8,
            0,
            wide.c_str(),
            -1,
            NULL,
            0,
            NULL,
            NULL);
        if (charcount == 0)
        {
            return std::string();
        }

        char *buf = new char[charcount];
        WideCharToMultiByte(
            CP_UTF8,
            0,
            wide.c_str(),
            -1,
            buf,
            charcount,
            NULL,
            NULL);

        std::string result(buf);
        delete[] buf;
        return result;
    }

}

#endif