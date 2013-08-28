#pragma once

class ZlResFont
{
public:
    HFONT GetFont(const wchar_t* fontName)
    {
        static HFONT hftRet = NULL;
        if (NULL == hftRet)
        {
            LOGFONT lfNew;
            ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lfNew);
            lfNew.lfWeight = FW_NORMAL;
            lfNew.lfUnderline = FALSE;
            lfNew.lfItalic = FALSE;
            lfNew.lfHeight = 12;
            lfNew.lfQuality = CLEARTYPE_QUALITY;
            wcscpy_s(lfNew.lfFaceName, L"Microsoft YaHei");
            hftRet = ::CreateFontIndirect(&lfNew);
        }
        return hftRet;
    }
};