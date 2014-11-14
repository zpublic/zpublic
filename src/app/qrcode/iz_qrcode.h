/**
 * @file
 * @brief 二维码生成接口
 */
#pragma once

#include <unknwn.h>

///> 二维码的错误修正等级
typedef enum __ZQrcodeLevel
{
    ZQrcodeLevel_Null            = 0,
    ZQrcodeLevel_l               = 1,           ///> 低
    ZQrcodeLevel_m               = 2,
    ZQrcodeLevel_q               = 3,
    ZQrcodeLevel_h               = 4,           ///> 高
}ZQrcodeLevel;

///> 二维码的类型
typedef enum __ZQrcodeMode
{
    ZQrcodeMode_Null             = 0,
    ZQrcodeMode_Num              = 1,           ///> Numeric
    ZQrcodeMode_An               = 2,           ///> Alphabet-numeric
    ZQrcodeMode_8                = 3,           ///> 8-bit data
    ZQrcodeMode_Kanji            = 4,           ///> Kanji (shift-jis)
    ZQrcodeMode_Structre         = 5,           ///> Internal use only.
    ZQrcodeMode_ECI              = 6,           ///> ECI
    ZQrcodeMode_Fun1first        = 7,           ///> FNC1, first position.
    ZQrcodeMode_Fun1Second       = 8,           ///> FNC1, second position.
}ZQrcodeMode;

typedef struct __ZQRcodeData
{
    int            width;        ///> 二维码的宽度
    BYTE*          data;         ///> 数组 width * width
}ZQRcodeData;

[
    uuid("594f31d0-7f19-11d0-b194-00a0c90dc8bf")
]
interface IZQrCode : public IUnknown
{
    /**
    * @brief 生成二维码
    * @param[in] qrstr 需要生成二维码的数据
    * @param[in] level 二维码的生成精度
    * @param[in] mode  二维码的生成模式
    * @return 成功返回ZQRcodeData结构体的智能, 失败NULL, 必须使用CloseQrcode释放内存
    */
    virtual ZQRcodeData*     CreateQrcode(const char* qrstr, ZQrcodeLevel level, ZQrcodeMode mode) = 0;
    /**
    * @brief 释放二维码结构
    * @param[in] qrfree 需要释放的ZQRcodeData指针
    */
    virtual void             CloseQrcode(ZQRcodeData* qrfree) = 0;
};
