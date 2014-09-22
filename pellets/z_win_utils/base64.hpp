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
 * @brief base64算法
 */



#pragma once
#include <string>


namespace zl
{
namespace WinUtils
{

#define BASE64_8BIT 3
#define BASE64_6BIT 4
#define base64_encode_length(A) (((A + BASE64_8BIT - 1) / BASE64_8BIT) * BASE64_6BIT + 1)
#define base64_decode_length(A) (((A + BASE64_6BIT - 1) / BASE64_6BIT) * BASE64_8BIT)

static const char _base64_dict[]         = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static const char _base64_websafe_dict[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";

static const unsigned int _base64_table[0x80] = 
{
    /*00-07*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*08-0f*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*10-17*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*18-1f*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*20-27*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*28-2f*/0xFF, 0xFF, 0xFF, 0x3e, 0xFF, 0xFF, 0xFF, 0x3f, //2 = '+' and '/'
    /*30-37*/0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, //8 = '0'-'7'
    /*38-3f*/0x3c, 0x3d, 0xFF, 0xFF, 0xFF, 0x40, 0xFF, 0xFF, //2 = '8'-'9' and '='
    /*40-47*/0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //7 = 'A'-'G'
    /*48-4f*/0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, //8 = 'H'-'O'
    /*50-57*/0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, //8 = 'P'-'W'
    /*58-5f*/0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //3 = 'X'-'Z'
    /*60-67*/0xFF, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, //7 = 'a'-'g'
    /*68-6f*/0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, //8 = 'h'-'o'
    /*70-77*/0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, //8 = 'p'-'w'
    /*78-7f*/0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF //3 = 'x'-'z'
};

static const unsigned int _base64_websafe_table[0x80] = 
{
    /*00-07*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*08-0f*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*10-17*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*18-1f*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*20-27*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /*28-2f*/0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3e, 0xFF, 0xFF, //1 = '-'
    /*30-37*/0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, //8 = '0'-'7'
    /*38-3f*/0x3c, 0x3d, 0xFF, 0xFF, 0xFF, 0x40, 0xFF, 0xFF, //2 = '8'-'9' and '='
    /*40-47*/0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //7 = 'A'-'G'
    /*48-4f*/0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, //8 = 'H'-'O'
    /*50-57*/0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, //8 = 'P'-'W'
    /*58-5f*/0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0x3f, //4 = 'X'-'Z' and '_'
    /*60-67*/0xFF, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, //7 = 'a'-'g'
    /*68-6f*/0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, //8 = 'h'-'o'
    /*70-77*/0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, //8 = 'p'-'w'
    /*78-7f*/0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF //3 = 'x'-'z'
};

/**
 * @brief base64算法
 */
class ZLBase64
{
public:
    // 标准的base64编码
    static std::string Encode(const char* s, size_t len);
    static std::string Decode(const char* s, size_t len);
    static size_t      DecodeBuffer(char* dest, size_t destsize,const char* src, size_t srcsize);

    // 将"+/"替换为"-_"
    static std::string WebSafeEncode(const char* s, size_t len);
    static std::string WebSafeDecode(const char* s, size_t len);

private:
    static size_t _EncodeImpl(char* dest, const char* src, size_t size, const char* dict);
    static size_t _DecodeImpl(char* dest, const char* src, size_t size, const unsigned int* base64table);
};

inline size_t ZLBase64::_EncodeImpl(char* dest, const char* src, size_t size, const char* dict)
{
    unsigned char* usrc = (unsigned char*) src;
    unsigned char* udest = (unsigned char*) dest;
    size_t destsize = 0;

    while (size >= 1)
    {
        size_t blocksize = __min(size, BASE64_8BIT);
        // Encode inputs...
        unsigned char n1, n2 = 0, n3 = 0, n4 = 0;
        switch (blocksize)
        {
        case 3:
            n4 = (usrc[2] & 0x3f);
            n3 = ((usrc[2] & 0xc0) >> 6);
        case 2:
            n3 |= ((usrc[1] & 0x0f) << 2);
            n2 = ((usrc[1] & 0xf0) >> 4);
        case 1:
            n2 |= ((usrc[0] & 0x03) << 4);
            n1 = ((usrc[0] & 0xfc) >> 2);
            break;

        default:
            break;
        }
        usrc += blocksize;
        size -= blocksize;

        // Padding...
        switch (blocksize)
        {
        case 1:
            n3 = 64;
        case 2:
            n4 = 64;
        case 3:
            break;

        default:
            break;
        }

        // 4 outputs...
        *udest++ = dict[n1];
        *udest++ = dict[n2];
        *udest++ = dict[n3];
        *udest++ = dict[n4];
        destsize += BASE64_6BIT; //4
    }
    return destsize;
}

inline size_t ZLBase64::_DecodeImpl(char* dest, const char* src, size_t size, const unsigned int* base64table)
{
    if (size < BASE64_6BIT || size % BASE64_6BIT != 0)
        return 0;

    unsigned char* usrc = (unsigned char*) src;
    unsigned char* udest = (unsigned char*) dest;
    size_t destsize = 0;

    while (size >= 1)
    {
        // 4 inputs...
        unsigned char in1 = *usrc++;
        unsigned char in2 = *usrc++;
        unsigned char in3 = *usrc++;
        unsigned char in4 = *usrc++;
        size -= BASE64_6BIT; //4

        // Convert ascii to base16...
        in1 = base64table[in1];
        in2 = base64table[in2];
        in3 = base64table[in3];
        in4 = base64table[in4];

        // 3 outputs...
        *udest++ = ((in1 & 0x3f) << 2) | ((in2 & 0x30) >> 4);
        *udest++ = ((in2 & 0x0f) << 4) | ((in3 & 0x3c) >> 2);
        *udest++ = ((in3 & 0x03) << 6) | (in4 & 0x3f);
        destsize += BASE64_8BIT; //3

        // Padding...
        if (in4 == 64)
        {
            --destsize;
            if (in3 == 64)
            {
                --destsize;
            }
        }
    }
    return destsize;
}

inline std::string ZLBase64::Encode(const char* s, size_t len)
{
    std::string temp;
    if (!s || len == 0)
    {
        return temp;
    }
    temp.resize(base64_encode_length(len));
    size_t output_size = _EncodeImpl(&(temp[0]), s, len, _base64_dict);
    temp.resize(output_size);
    return temp;
}

inline std::string ZLBase64::Decode(const char* s, size_t len)
{
    std::string temp;
    if (!s || len == 0)
    {
        return temp;
    }
    temp.resize(base64_decode_length(len));
    size_t output_size = _DecodeImpl(&(temp[0]), s, len, _base64_table);
    temp.resize(output_size);
    return temp;
}

inline std::string ZLBase64::WebSafeEncode(const char* s, size_t len)
{
    std::string temp;
    if (!s || len == 0)
    {
        return temp;
    }
    temp.resize(base64_encode_length(len));
    size_t output_size = _EncodeImpl(&(temp[0]), s, len, _base64_websafe_dict);
    temp.resize(output_size);
    return temp;
}

inline std::string ZLBase64::WebSafeDecode(const char* s, size_t len)
{
    std::string temp;
    if (!s || len == 0)
    {
        return temp;
    }
    temp.resize(base64_decode_length(len));
    size_t output_size = _DecodeImpl(&(temp[0]), s, len, _base64_websafe_table);
    temp.resize(output_size);
    return temp;
}

inline size_t ZLBase64::DecodeBuffer(char* dest, size_t destsize,const char* src, size_t srcsize)
{
    if (!dest || !src || srcsize == 0 || destsize == 0)
    {
        return 0;
    }
    if ( base64_decode_length(srcsize) > destsize )
    {
        return 0;
    }
    size_t output_size = _DecodeImpl(dest, src, srcsize, _base64_table);
    return output_size;
}


} // end of namespace WinUtils
} // end of namespace zl
