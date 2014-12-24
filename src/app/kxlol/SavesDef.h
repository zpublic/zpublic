#pragma once

///> 存档由头部、扩展头部和内容三部分构成

typedef struct _KXLOL_SAVES_DATA
{
    ULONGLONG   m_ullShenli;
}KXLOL_SAVES_DATA;

typedef struct _KXLOL_SAVES_EXT_HEAD
{
}KXLOL_SAVES_EXT_HEAD;

typedef struct _KXLOL_SAVES_HEAD
{
    _KXLOL_SAVES_HEAD()
    {
        dwSign          = 'kxkx';
        dwVerMajor      = 1;
        dwVerMinor      = 0;
        dwExtSize       = 0;
        dwDataSize      = sizeof(KXLOL_SAVES_DATA);
    }

    DWORD       dwSign;
    DWORD       dwVerMajor;
    DWORD       dwVerMinor;
    DWORD       dwExtSize;
    DWORD       dwDataSize;
}KXLOL_SAVES_HEAD;
