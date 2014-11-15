#ifndef _APP_ZQRCODE
#define _APP_ZQRCODE

#include "iz_qrcode.h"
#include "pellets\z_com\z_com_export.h"
#include "qrencode\qrencode.h"

class ZQrCode : public IZQrCode
{
public:
    ZQrCode();
    ~ZQrCode();

public:
    virtual ZQRcodeData*     CreateQrcode(const char* qrstr, ZQrcodeLevel level, ZQrcodeMode mode);
    virtual void             CloseQrcode(ZQRcodeData* qrfree);

public:
    ZL_BEGIN_COM_MAP(ZQrCode)
        ZL_COM_INTERFACE_ENTRY(IZQrCode)
    ZL_END_COM_MAP()

private:
    QRecLevel               _Get2Level(ZQrcodeLevel level);
    QRencodeMode            _Get2Mode(ZQrcodeMode mode);
    ZQRcodeData*            _TransitionQrcode(QRcode* retRenCode);
};

#endif