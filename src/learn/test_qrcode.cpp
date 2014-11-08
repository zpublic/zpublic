#include "stdafx.h"
#include "test_qrcode.h"
#include "qrencode/qrencode.h"

void test_qrcode()
{
    QRecLevel qrlevel = QR_ECLEVEL_M;
    QRencodeMode qrmod = QR_MODE_8;
    QRcode *qrcode = QRcode_encodeString("zapline", 1, qrlevel, qrmod, 1);
    if (qrcode)
    {
        for (int i=0; i<qrcode->width; ++i)
        {
            for (int j=0; j<qrcode->width; ++j)
            {
                char c = (*(qrcode->data + (i*qrcode->width) + j) & 0x1) ? '1' : '0';
                std::cout<<c;
            }
            std::cout<<std::endl;
        }
    }
}
