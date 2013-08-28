#pragma once

#include "zl_res_font.h"

class ZlRes
{
public:
    static ZlRes& Instance()
    {
        static ZlRes inst;
        return inst;
    }

    ZlResFont& Font()
    {
        return m_font;
    }

private:
    ZlResFont m_font;
};