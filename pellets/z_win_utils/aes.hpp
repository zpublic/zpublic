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
 * @brief AES算法实现
 */

#pragma once
#include <string.h>
#include "rijndael.hpp"

namespace zl
{
namespace WinUtils
{

/**
 * @brief AES(Advanced Encryption Standard)高级加密标准
 *
 *  仅提供2个接口:Encrypt和Decrypt,分别用于AES加密和解密,
 * @note: 注:待加密内容和输出内容,可以是同一块内存区.方便使用.但请预留足够大小存放输出内容.(比输入长度大16即可)
 * @todo 坑:PADDING_PKCS7, CFB, OFB尚未实现
 */
class ZLAes
{
public:
    enum KeyBits { KEY_BITS_128 = 128, KEY_BITS_192 = 192, KEY_BITS_256 = 256 };
    enum PaddingMode
    {
        // The number of bytes to be padded equals to "8 - numberOfBytes(clearText) mod 8".
        // So 1 to 8 bytes will be padded to the clear text data depending on the length of the clear text data.
        // All padded bytes have the same value - the number of bytes padded.
        // If numberOfBytes(clearText) mod 8 == 7, PM = M + 0x01
        // If numberOfBytes(clearText) mod 8 == 6, PM = M + 0x0202
        // If numberOfBytes(clearText) mod 8 == 5, PM = M + 0x030303
        // ...
        // If numberOfBytes(clearText) mod 8 == 0, PM = M + 0x0808080808080808
        PADDING_PKCS5,
        
        // 填充字符串由一个字节序列组成，每个字节填充该字节序列的长度
        // 下面的示例演示这些模式的工作原理。假定块长度为 8，数据长度为 9，则填充用八位字节数等于 7，数据等于 FF FF FF FF FF FF FF FF FF：
        // 数据: FF FF FF FF FF FF FF FF FF
        // PKCS7 填充: FF FF FF FF FF FF FF FF FF 07 07 07 07 07 07 07
        PADDING_PKCS7,
    };

    enum BlockMode
    {
        ECB, // Electronic Code Book,是一种基础的加密方式,密文被分割成分组长度相等的块(不足补齐),然后单独一个个加密,一个个输出组成密文.
        CBC, // Cipher Block Chaining,是一种循环模式,前一个分组的密文和当前分组的明文异或操作后再加密,这样做的目的是增强破解难度.
        CFB, // Cipher Feed Back,实际上是一种反馈模式，目的也是增强破解的难度。
        OFB, // Output Feed Back,实际上是一种反馈模式，目的也是增强破解的难度。
    };

public:
    /**
     * @brief AES加密
     * @param[in]  block_mode
     * @param[in]  padding_mode
     * @param[in]  key_bits 密钥长度分为 128位，192位，256位
     * @param[in]  key 密码
     * @param[in]  src 待加密内容
     * @param[in]  src_size 待加密内容大小
     * @param[out] dest 加密结果缓存区,请预留足够大小存放输出内容.(比输入长度大16即可)
     * @param[in]  dest_size 加密结果缓存区大小
     * @return 加密后的缓存区大小
     * @note src和dest可以是同一缓存区
     */
    static size_t Encrypt(
        const BlockMode      block_mode,
        const PaddingMode    padding_mode,
        const KeyBits        key_bits,
        const unsigned char* key,
        const unsigned char* src,
        const size_t         src_size,
        unsigned char*       dest,
        const size_t         dest_size);

    /**
     * @brief AES解密
     * @param[in]  block_mode
     * @param[in]  padding_mode
     * @param[in]  key_bits 密钥长度分为 128位，192位，256位
     * @param[in]  key 密码
     * @param[in]  src 待解密内容
     * @param[in]  src_size 待解密内容大小
     * @param[out] dest 解密结果缓存区
     * @param[in]  dest_size 解密结果缓存区大小
     * @return 解密后的缓存区大小
     * @note src和dest可以是同一缓存区
     */
    static size_t Decrypt(
        const BlockMode      block_mode,
        const PaddingMode    padding_mode,
        const KeyBits        key_bits,
        const unsigned char* key,
        const unsigned char* src,
        const size_t         src_size,
        unsigned char*       dest,
        const size_t         dest_size);

private:
    // Padding
    static size_t _Padding(const PaddingMode padding_mode, const unsigned char* src, const size_t src_size, unsigned char* dest, const size_t dest_size);
    static size_t _PaddingPKCS5(const unsigned char* src, const size_t src_size, unsigned char* dest, const size_t dest_size);

    // De-padding
    static size_t _DePadding(const PaddingMode padding_mode, const unsigned char* src, const size_t src_size, unsigned char* dest, const size_t dest_size);
    static size_t _DePaddingPKCS5(const unsigned char* src, const size_t src_size, unsigned char* dest, const size_t dest_size);

    // Encrypt
    static bool _Encrypt(BlockMode block_mode, const unsigned char* key, const KeyBits key_bits, const unsigned char* in, const size_t in_size, unsigned char* out);
    static void _EcbEncrypt(const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out);
    static void _CbcEncrypt(const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out);

    // Decrypt
    static bool _Decrypt(BlockMode block_mode, const unsigned char* key, const KeyBits key_bits, const unsigned char* in, const size_t in_size, unsigned char* out);
    static void _EcbDecrypt(const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out);
    static void _CbcDecrypt(const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out);
};

inline size_t ZLAes::Encrypt(
    const BlockMode      block_mode,
    const PaddingMode    padding_mode,
    const KeyBits        key_bits,
    const unsigned char* key,
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    size_t ret = 0;

    // Step1: 校验各个参数
    if (NULL == key   ||
        NULL == src   ||
        NULL == dest  ||
        src_size <= 0 ||
        (src_size + 16) > dest_size)
    {
        goto Exit0;
    }

    // Step2: Padding
    size_t real_size = _Padding(padding_mode, src, src_size, dest, dest_size);
    if (real_size < src_size ||
        real_size % 16 != 0)
    {
        goto Exit0;
    }

    // Step3: Encrypt
    if (_Encrypt(block_mode, key, key_bits, dest, real_size, dest))
    {
        ret = real_size;
    }

Exit0:
    return ret;
}

inline size_t ZLAes::Decrypt(
    const BlockMode      block_mode,
    const PaddingMode    padding_mode,
    const KeyBits        key_bits,
    const unsigned char* key,
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    size_t ret = 0;
    // Step1: 校验各个参数
    if (NULL == key   ||
        NULL == src   ||
        NULL == dest  ||
        src_size <= 0 ||
        (src_size > dest_size))
    {
        goto Exit0;
    }

    // Step2: Decrypt
    if (_Decrypt(block_mode, key, key_bits, src, src_size, dest))
    {
        // Step3: De-padding
        size_t real_size = _DePadding(padding_mode, dest, src_size, dest, dest_size);
        ret = real_size;
    }

Exit0:
    return ret;
}

inline size_t ZLAes::_Padding(
    const PaddingMode    padding_mode,
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    switch (padding_mode)
    {
    case PADDING_PKCS5:
        return _PaddingPKCS5(src, src_size, dest, dest_size);
    }
    return 0;
}

inline size_t ZLAes::_PaddingPKCS5(
                                   const unsigned char* src,
                                   const size_t         src_size,
                                   unsigned char*       dest,
                                   const size_t         dest_size
                                   )
{
    size_t need_size =  ((src_size + 16) / 16) * 16;
    if (need_size <= dest_size)
    {
        memcpy_s(dest, dest_size, src, src_size);
        size_t value = need_size - src_size;
        for (size_t i = 0; i < value; i++)
        {
            dest[src_size + i] = (unsigned char)value;
        }
    }
    else
    {
        need_size = 0;
    }
    return need_size;
}

inline size_t ZLAes::_DePadding(
    const PaddingMode    padding_mode,
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    switch (padding_mode)
    {
    case PADDING_PKCS5:
        return _DePaddingPKCS5(src, src_size, dest, dest_size);
    }
    return 0;
}

inline size_t ZLAes::_DePaddingPKCS5(
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    size_t padding_value = *(src + src_size - 1);
    size_t valid_size = src_size - padding_value;

    memcpy_s(dest, dest_size, src, valid_size);
    return valid_size;
}

inline bool ZLAes::_Encrypt(
    BlockMode            block_mode,
    const unsigned char* key,
    const KeyBits        key_bits,
    const unsigned char* in,
    const size_t         in_size,
    unsigned char*       out
    )
{
    bool suc = false;
    unsigned long rk[ZLRIJNDAEL_RK_SIZE] = {0};
    int rounds = ZLRijndael::SetupEncrypt(rk, key, (int)key_bits);
    if (0 != rounds)
    {
        suc = true;
        switch (block_mode)
        {
        case ECB:
            _EcbEncrypt(rk, rounds, in, in_size, out);
            break;
        case CBC:
            _CbcEncrypt(rk, rounds, in, in_size, out);
            break;
        default:
            suc = false;
            break;
        }
    }
    return suc;
}

inline bool ZLAes::_Decrypt(
    BlockMode            block_mode,
    const unsigned char* key,
    const KeyBits        key_bits,
    const unsigned char* in,
    const size_t         in_size,
    unsigned char*       out
    )
{
    bool suc = false;
    unsigned long rk[ZLRIJNDAEL_RK_SIZE] = {0};
    int rounds = ZLRijndael::SetupDecrypt(rk, key, (int)key_bits);
    if (0 != rounds)
    {
        suc = true;
        switch (block_mode)
        {
        case ECB:
            _EcbDecrypt(rk, rounds, in, in_size, out);
            break;
        case CBC:
            _CbcDecrypt(rk, rounds, in, in_size, out);
            break;
        default:
            suc = false;
            break;
        }
    }
    return suc;
}

inline void ZLAes::_EcbEncrypt(
    const unsigned long* rk,
    const int            rounds,
    const unsigned char* in,
    const size_t         in_size,
    unsigned char*       out
    )
{
    size_t block_count = in_size / 16;
    while (block_count)
    {
        ZLRijndael::Encrypt(rk, rounds, in, out);
        in  += 16;
        out += 16;
        block_count -= 1;
    }
}

inline void ZLAes::_CbcEncrypt(
    const unsigned long* rk,
    const int            rounds,
    const unsigned char* in,
    const size_t         in_size,
    unsigned char*       out
    )
{
    unsigned char buffer[16] = {0};
    size_t block_count = in_size / 16;
    while (block_count)
    {
        for (unsigned int pos = 0; pos < 16; ++pos)
            buffer[pos] ^= *in++;
        ZLRijndael::Encrypt(rk, rounds, buffer, out);
        memcpy(buffer, out, 16);
        out += 16;
        block_count -= 1;
    }
}

inline void ZLAes::_EcbDecrypt(
                               const unsigned long* rk,
                               const int            rounds,
                               const unsigned char* in,
                               const size_t         in_size,
                               unsigned char*       out
                               )
{
    size_t block_count = in_size / 16;
    while (block_count)
    {
        ZLRijndael::Decrypt(rk, rounds, in, out);
        in  += 16;
        out += 16;
        block_count -= 1;
    }
}

inline void ZLAes::_CbcDecrypt( const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out )
{
    size_t block_count = in_size / 16;

    unsigned char buffer[16] = {0};
    memset(buffer, 0, sizeof(buffer));
    ZLRijndael::Decrypt(rk, rounds, in, out);
    for (unsigned int pos = 0; pos < 16; ++pos)
        *out++ ^= buffer[pos];
    in += 16;
    block_count--;

    while (block_count)
    {
        ZLRijndael::Decrypt(rk, rounds, in, out);
        for (unsigned int pos = 0; pos < 16; ++pos)
            *out++ ^= *(in- 16 + pos);
        in += 16;
        --block_count;
    }
}

} // end of WinUtils
} // end of zl