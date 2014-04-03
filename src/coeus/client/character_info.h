#pragma once

class CharacterInfo
{
public:
    typedef struct _Properties
    {
        uint64 coins;
        uint32 exp;
        uint32 level;
    }Properties;

public:
    CharacterInfo();

    void SetCallbackDlg(HWND hWnd);

    void PropertiesReq();

    void NotifyProperties(uint64 coins, uint32 exp, uint32 level);

private:
    HWND    m_hDlg;
};
