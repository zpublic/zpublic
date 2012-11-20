/********************************************************************
* @file      : guid.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/20 9:28
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _GUID_HPP_
#define _GUID_HPP_

#define kGUIDLength 36U
#define kGUIDSize   39U

namespace zl
{

    template <typename StringType>
    bool IsValidGUID(const StringType& guid)
    {
        if (guid.length() != kGUIDLength)
            return false;

        for (size_t i = 0; i < guid.length(); ++i)
        {
            StringType::traits_type::_Elem current = guid[i];
            if (i == 8 || i == 13 || i == 18 || i == 23)
            {
                if (current != '-')
                    return false;
            }
            else
            {
                if ((current < '0' || current > '9')
                    && (current < 'A' || current > 'F'))
                {
                    return false;
                }
            }
        }
        return true;
    }

    template <const char*>
    bool IsValidGUID(const char* guid)
    {
        if (NULL = guid)
            return false;
        size_t nGuidLen = ::strlen(guid);
        if (nGuidLen != kGUIDLength)
            return false;

        for (size_t i = 0; i < nGuidLen; ++i)
        {
            char current = guid[i];
            if (i == 8 || i == 13 || i == 18 || i == 23)
            {
                if (current != '-')
                {
                    return false;
                }
            }
            else
            {
                if ((current < '0' || current > '9')
                    && (current < 'A' || current > 'F'))
                {
                    return false;
                }
            }
        }
        return true;
    }

    std::wstring GenerateGUID()
    {
        wchar_t szGuid[kGUIDSize] = {0};
        GUID guid;
        HRESULT guid_result = ::CoCreateGuid(&guid);
        if (!SUCCEEDED(guid_result))
            return std::wstring();

        int result = ::StringFromGUID2(
            guid,
            szGuid,
            kGUIDSize);
        if (result != kGUIDSize)
            return std::wstring();

        std::wstring guid_string(szGuid);
        return guid_string.substr(1, guid_string.length() - 2);
    }

    std::wstring GUIDToWString(GUID *guid)
    {
        wchar_t guid_string[kGUIDLength + 1] = {0};
        ::swprintf(
            guid_string, sizeof(guid_string) / sizeof(guid_string[0]),
            L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            guid->Data1, guid->Data2, guid->Data3,
            guid->Data4[0], guid->Data4[1], guid->Data4[2],
            guid->Data4[3], guid->Data4[4], guid->Data4[5],
            guid->Data4[6], guid->Data4[7]);

        guid_string[sizeof(guid_string) / sizeof(guid_string[0]) - 1] = L'\0';

        return std::wstring(guid_string);
    }

}

#endif