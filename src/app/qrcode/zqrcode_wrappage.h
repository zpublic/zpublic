#pragma once

#include "iz_qrcode.h"
#include "z_com\z_com.h"

class ZQRcodeWrappage
{
public:
    ZQRcodeWrappage()
    {
        m_qrcode = NULL;
    }
    ~ZQRcodeWrappage()
    {

    }

public:
    BOOL Init(LPCTSTR pszDLLPath)
    {
        if (SUCCEEDED(m_loader.Initialize(pszDLLPath)))
        {
            m_qrcode = m_loader.GetInterface();
        }
        return m_qrcode ? TRUE : FALSE;
    }

    BOOL Uint()
    {
        m_loader.UnInitialize();
        if (m_qrcode)
        {
            m_qrcode->Release();
            m_qrcode = NULL;
        }
        return TRUE;
    }

    ZQRcodeData* QrcodeEncode(const char* qrstr, ZQrcodeLevel level = ZQrcodeLevel_m, ZQrcodeMode mode = ZQrcodeMode_8)
    {
        ZQRcodeData* pData = NULL;
        if (qrstr && m_qrcode)
        {
            pData = m_qrcode->CreateQrcode(qrstr, level, mode);
        }
        return pData;
    }

    void CloseQrcode(ZQRcodeData* qrcode)
    {
        if (m_qrcode)
        {
            m_qrcode->CloseQrcode(qrcode);
        }
    }

private:
    IZQrCode* m_qrcode;
    ZLComLoader<IZQrCode> m_loader;
};