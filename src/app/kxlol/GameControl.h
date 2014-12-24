#pragma once
#include "PlayerObject.h"
#include "SavesMgr.h"
#include "LogicMgr.h"

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
    CPlayerObject& Player()
    {
        return m_cPlayer;
    }

    CSavesMgr& Saves()
    {
        return m_cSaves;
    }

    CLogicMgr& Logic()
    {
        return m_cLogic;
    }

private:
    CPlayerObject           m_cPlayer;
    CSavesMgr               m_cSaves;
    CLogicMgr               m_cLogic;
};

