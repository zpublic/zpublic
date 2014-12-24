#pragma once
#include "PlayerObject.h"
#include "SavesMgr.h"
#include "LogicMgr.h"
#include "ScriptMgr.h"

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

    CScriptMgr& Script()
    {
        return m_cScript;
    }

private:
    CPlayerObject           m_cPlayer;
    CSavesMgr               m_cSaves;
    CLogicMgr               m_cLogic;
    CScriptMgr              m_cScript;
};

