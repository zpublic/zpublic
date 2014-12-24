#pragma once
#include "PlayerObject.h"

class CGameControl
{
public:
    CGameControl();
    ~CGameControl();

    static CGameControl& Instance()
    {
        static CGameControl c;
        return c;
    }

public:
    CPlayerObject* Player()
    {
        return &m_cPlayer;
    }

    CSavesMgr* Saves()
    {
        return &m_cSaves;
    }

private:
    CPlayerObject           m_cPlayer;
    CSavesMgr               m_cSaves;
};

