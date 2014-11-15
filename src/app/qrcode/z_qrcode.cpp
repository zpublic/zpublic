#include "stdafx.h"
#include "z_qrcode.h"

#define ZQREN_VER               1

ZQrCode::ZQrCode()
{

}

ZQrCode::~ZQrCode()
{

}

ZQRcodeData* ZQrCode::CreateQrcode(const char* qrstr, ZQrcodeLevel level, ZQrcodeMode mode)
{
    ZQRcodeData* retZCode = NULL;
    QRcode* retRenCode = QRcode_encodeString(qrstr, ZQREN_VER, _Get2Level(level), _Get2Mode(mode), 1);
    if (retRenCode)
    {
        retZCode = _TransitionQrcode(retRenCode);
        QRcode_free(retRenCode);
        retRenCode = NULL;
    }
    return retZCode;
}

void ZQrCode::CloseQrcode(ZQRcodeData* qrfree)
{
    if (qrfree)
    {
        if (qrfree->data)
        {
            delete[] qrfree->data;
            qrfree->data = NULL;
        }
        delete qrfree;
        qrfree = NULL;
    }
}

QRecLevel ZQrCode::_Get2Level(ZQrcodeLevel level)
{
    switch (level)
    {
    case ZQrcodeLevel_l: return QR_ECLEVEL_L;
    case ZQrcodeLevel_m: return QR_ECLEVEL_M;
    case ZQrcodeLevel_q: return QR_ECLEVEL_Q;
    case ZQrcodeLevel_h: return QR_ECLEVEL_H;
    }
    return QR_ECLEVEL_L;
}

QRencodeMode ZQrCode::_Get2Mode(ZQrcodeMode mode)
{
    switch (mode)
    {
    case ZQrcodeMode_Null      : return QR_MODE_NUL;
    case ZQrcodeMode_Num       : return QR_MODE_NUM;
    case ZQrcodeMode_An        : return QR_MODE_AN;
    case ZQrcodeMode_8         : return QR_MODE_8;
    case ZQrcodeMode_Kanji     : return QR_MODE_KANJI;
    case ZQrcodeMode_Structre  : return QR_MODE_STRUCTURE;
    case ZQrcodeMode_ECI       : return QR_MODE_ECI;
    case ZQrcodeMode_Fun1first : return QR_MODE_FNC1FIRST;
    case ZQrcodeMode_Fun1Second: return QR_MODE_FNC1SECOND;
    }
    return QR_MODE_NUL;
}

ZQRcodeData* ZQrCode::_TransitionQrcode(QRcode* retRenCode)
{
    ZQRcodeData* retZCode = new ZQRcodeData;
    if (retZCode)
    {
        retZCode->data = new BYTE[retRenCode->width * retRenCode->width];
        if (retZCode->data != NULL)
        {
            memcpy(retZCode->data, retRenCode->data, retRenCode->width * retRenCode->width);
            retZCode->width = retRenCode->width;
        }
        else
        {
            CloseQrcode(retZCode);
            retZCode = NULL;
        }
    }
    return retZCode;
}
