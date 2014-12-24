#pragma once
#include "LogicBuild.h"
#include "LogicMake.h"
#include "LogicPractice.h"
#include "LogicGui.h"

class CLogicMgr
{
public:
    CLogicMgr();
    ~CLogicMgr();

    CLogicBuild& Build()
    {
        return m_cBuild;
    }

    CLogicMake& Make()
    {
        return m_cMake;
    }

    CLogicPractice& Practice()
    {
        return m_cPractice;
    }

    CLogicGui& Gui()
    {
        return m_cGui;
    }

private:
    CLogicBuild             m_cBuild;
    CLogicMake              m_cMake;
    CLogicPractice          m_cPractice;
    CLogicGui               m_cGui;
};
